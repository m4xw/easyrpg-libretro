/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include "game_player.h"
#include "async_handler.h"
#include "game_actor.h"
#include "game_map.h"
#include "game_message.h"
#include "game_party.h"
#include "game_system.h"
#include "game_temp.h"
#include "graphics.h"
#include "input.h"
#include "main_data.h"
#include "player.h"
#include "util_macro.h"
#include "game_switches.h"
#include "output.h"
#include "reader_util.h"
#include "scope_guard.h"
#include <algorithm>
#include <cmath>

Game_Player::Game_Player():
	Game_Character(Player, &Main_Data::game_data.party_location)
{
	SetDirection(RPG::EventPage::Direction_down);
	SetMoveSpeed(4);
	SetAnimationType(RPG::EventPage::AnimType_non_continuous);
}

int Game_Player::GetScreenZ(bool apply_shift) const {
	// Player is always slightly above events
	// (and always on "same layer as hero" obviously)
	return Game_Character::GetScreenZ(apply_shift) + 1;
}

bool Game_Player::GetVisible() const {
	return visible && !data()->aboard;
}

void Game_Player::ReserveTeleport(int map_id, int x, int y, int direction) {
	teleport_target = TeleportTarget(map_id, x, y, direction);
}

void Game_Player::ReserveTeleport(const RPG::SaveTarget& target) {
	int map_id = target.map_id;

	if (Game_Map::GetMapType(target.map_id) == RPG::TreeMap::MapType_area) {
		// Area: Obtain the map the area belongs to
		map_id = Game_Map::GetParentId(target.map_id);
	}

	ReserveTeleport(map_id, target.map_x, target.map_y, Down);

	if (target.switch_on) {
		Game_Switches.Set(target.switch_id, true);
		Game_Map::SetNeedRefresh(Game_Map::Refresh_All);
	}
}

void Game_Player::PerformTeleport() {
	assert(IsPendingTeleport());
	if (!IsPendingTeleport()) {
		return;
	}

	if (teleport_target.GetMapId() <= 0) {
		Output::Error("Invalid Teleport map id! mapid=%d x=%d y=%d d=%d", teleport_target.GetMapId(),
				teleport_target.GetX(), teleport_target.GetY(), teleport_target.GetDirection());
	}

	// Reset sprite if it was changed by a move
	// Even when target is the same map
	Refresh();

	ResetAnimation();
	if (Game_Map::GetMapId() != teleport_target.GetMapId()) {
		Game_Map::Setup(teleport_target.GetMapId());
	} else {
		Game_Map::SetupFromTeleportSelf();
	}

	SetTransparency(0);

	MoveTo(teleport_target.GetX(), teleport_target.GetY());

	if (teleport_target.GetDirection() >= 0) {
		SetDirection(teleport_target.GetDirection());
		if (!IsFacingLocked()) {
			SetSpriteDirection(teleport_target.GetDirection());
		}
	}

	if (InVehicle()) {
		GetVehicle()->SyncWithPlayer();
	}

	teleport_target = {};
}

bool Game_Player::MakeWay(int x, int y) const {
	if (data()->aboard) {
		return GetVehicle()->MakeWay(x, y);
	}

	return Game_Character::MakeWay(x, y);
}

void Game_Player::MoveTo(int x, int y) {
	Game_Character::MoveTo(x, y);
	Game_Map::SetPositionX(GetSpriteX() - data()->pan_current_x);
	Game_Map::SetPositionY(GetSpriteY() - data()->pan_current_y);
}

void Game_Player::UpdateScroll(int old_x, int old_y) {
	if (Game_Map::IsPanLocked()) {
		return;
	}

	int screen_x = Game_Map::GetPositionX();
	int screen_y = Game_Map::GetPositionY();

	int old_panx = old_x - screen_x;
	int old_pany = old_y - screen_y;

	int new_x = GetSpriteX();
	int new_y = GetSpriteY();

	int dx = new_x - old_x;
	int dy = new_y - old_y;

	int new_panx = new_x - screen_x;
	int new_pany = new_y - screen_y;

	// Detect whether we crossed map boundary.
	// We need to scale down dx/dy to a single step
	// to not message up further calculations.
	// FIXME: This logic will break if something moves so fast
	// as to cross half the map in 1 frame.
	if (Game_Map::LoopHorizontal()) {
		auto w = Game_Map::GetWidth() * SCREEN_TILE_SIZE;
		if (std::abs(dx) > w / 2) {
			dx = (w - std::abs(dx)) % w;
			if (new_x > old_x) {
				dx = -dx;
			}
		}
	}
	if (Game_Map::LoopVertical()) {
		auto h = Game_Map::GetHeight() * SCREEN_TILE_SIZE;
		if (std::abs(dy) > h / 2) {
			dy = (h - std::abs(dy)) % h;
			if (new_y > old_y) {
				dy = -dy;
			}
		}
	}

	if (Game_Map::LoopHorizontal() ||
			std::abs(data()->pan_current_x - new_panx) >=
			std::abs(data()->pan_current_x - old_panx)) {
		Game_Map::ScrollRight(dx);
	}

	if (Game_Map::LoopVertical() ||
			std::abs(data()->pan_current_y - new_pany) >=
			std::abs(data()->pan_current_y - old_pany)) {
		Game_Map::ScrollDown(dy);
	}
}

void Game_Player::UpdateSelfMovement() {
	if (!Game_Map::GetInterpreter().IsRunning() && !Game_Map::IsAnyEventStarting()) {
		if (IsMovable()) {
			const auto old_x = GetX();
			const auto old_y = GetY();
			const bool force_through = (Player::debug_flag
					&& Input::IsPressed(Input::DEBUG_THROUGH)
					&& !GetThrough());
			if (force_through) {
				SetThrough(true);
			}
			auto sg = makeScopeGuard([&](){
					if (force_through) { SetThrough(false); }
					});

			switch (Input::dir4) {
				case 2:
					Move(Down);
					break;
				case 4:
					Move(Left);
					break;
				case 6:
					Move(Right);
					break;
				case 8:
					Move(Up);
			}
			if (GetX() != old_x || GetY() != old_y) {
				Main_Data::game_party->IncSteps();
			}
		}

		// ESC-Menu calling
		if (Game_System::GetAllowMenu() && !Game_Message::message_waiting && !IsMoveRouteOverwritten() && Input::IsTriggered(Input::CANCEL)) {
			Main_Data::game_data.party_location.menu_calling = true;
			Game_System::SePlay(Game_System::GetSystemSE(Game_System::SFX_Decision));
		}
	}
}

void Game_Player::OnMoveFailed(int x, int y) {
	if (IsMoveRouteOverwritten()) {
		return;
	}

	CheckEventTriggerThere({RPG::EventPage::Trigger_touched, RPG::EventPage::Trigger_collision}, x, y, true, false);
}

void Game_Player::Update() {
	if (IsProcessed()) {
		return;
	}
	SetProcessed(true);

	const auto old_sprite_x = GetSpriteX();
	const auto old_sprite_y = GetSpriteY();

	auto was_moving = !IsStopping();
	auto was_move_route_overriden = IsMoveRouteOverwritten();

	Game_Character::UpdateMovement();
	Game_Character::UpdateAnimation(was_moving);

	if (IsAboard()) {
		auto* vehicle = GetVehicle();
		if (vehicle) {
			GetVehicle()->SyncWithPlayer();
			vehicle->AnimateAscentDescent();
		}
	}

	UpdateScroll(old_sprite_x, old_sprite_y);

	if (IsMoving()) return;

	bool finished_boarding_or_unboarding = false;
	if (data()->boarding) {
		// Boarding completed
		data()->aboard = true;
		data()->boarding = false;
		auto* vehicle = GetVehicle();
		if (vehicle->IsMoveRouteOverwritten()) {
			vehicle->CancelMoveRoute();
		}
		SetMoveSpeed(vehicle->GetMoveSpeed());
		vehicle->SetDirection(GetDirection());
		vehicle->SetSpriteDirection(Left);
		// Note: RPG_RT ignores the lock_facing flag here!
		SetSpriteDirection(Left);
		vehicle->SetX(GetX());
		vehicle->SetY(GetY());
		finished_boarding_or_unboarding = true;
	}

	if (data()->unboarding) {
		// Unboarding completed
		data()->unboarding = false;
		finished_boarding_or_unboarding = true;
	}

	if (IsMoveRouteOverwritten()) return;

	if (!InAirship()) {
		TriggerSet triggers;

		if (!Game_Map::GetInterpreter().IsRunning()) {
			triggers[RPG::EventPage::Trigger_collision] = true;
		}

		// When the last command of a move route is a move command, there is special
		// logic to reset the move route index to 0. We leverage this here because
		// we only do touch checks if the last move command was a move.
		// Checking was_moving is not enough, because there could have been 0 frame
		// commands after the move in the move route, in which case index would be > 0.
		if (was_moving && (!was_move_route_overriden || GetMoveRouteIndex() == 0)) {
			triggers[RPG::EventPage::Trigger_touched] = true;
			triggers[RPG::EventPage::Trigger_collision] = true;
		}

		if (triggers.count() > 0 && CheckEventTriggerHere(triggers, true, false)) {
			return;
		}
	}

	if (finished_boarding_or_unboarding) {
		return;
	}

	if (!Game_Map::GetInterpreter().IsRunning() && !Game_Map::IsAnyEventStarting()) {
		if (!Game_Message::visible && Input::IsTriggered(Input::DECISION)) {
			if ( GetOnOffVehicle() ) return;
			if ( CheckActionEvent() ) return;
		}
	}

	if (was_moving) {
		Game_Map::UpdateEncounterSteps();
	}
}

bool Game_Player::CheckActionEvent() {
	if (IsFlying()) {
		return false;
	}

	bool result = CheckEventTriggerHere({RPG::EventPage::Trigger_action}, true, true);

	int front_x = Game_Map::XwithDirection(GetX(), GetDirection());
	int front_y = Game_Map::YwithDirection(GetY(), GetDirection());

	result |= CheckEventTriggerThere({RPG::EventPage::Trigger_touched, RPG::EventPage::Trigger_collision}, front_x, front_y, true, true);

	// Counter tile loop stops only if you talk to an action event.
	bool got_action = CheckEventTriggerThere({RPG::EventPage::Trigger_action}, front_x, front_y, true, true);
	// RPG_RT allows maximum of 3 counter tiles
	for (int i = 0; !got_action && i < 3; ++i) {
		if (!Game_Map::IsCounter(front_x, front_y)) {
			break;
		}

		front_x = Game_Map::XwithDirection(front_x, GetDirection());
		front_y = Game_Map::YwithDirection(front_y, GetDirection());

		got_action |= CheckEventTriggerThere({RPG::EventPage::Trigger_action}, front_x, front_y, true, true);
	}
	return result || got_action;
}

bool Game_Player::CheckEventTriggerHere(TriggerSet triggers, bool face_hero, bool triggered_by_decision_key) {
	bool result = false;

	std::vector<Game_Event*> events;
	Game_Map::GetEventsXY(events, GetX(), GetY());

	for (auto* ev: events) {
		if (ev->GetLayer() != RPG::EventPage::Layers_same
				&& triggers[ev->GetTrigger()]) {
			result |= ev->SetAsWaitingForegroundExecution(face_hero, triggered_by_decision_key);
		}
	}
	return result;
}

bool Game_Player::CheckEventTriggerThere(TriggerSet triggers, int x, int y, bool face_hero, bool triggered_by_decision_key) {
	bool result = false;

	std::vector<Game_Event*> events;
	Game_Map::GetEventsXY(events, x, y);

	for (const auto& ev : events) {
		if ( ev->GetLayer() == RPG::EventPage::Layers_same && triggers[ev->GetTrigger()]) {
			result |= ev->SetAsWaitingForegroundExecution(face_hero, triggered_by_decision_key);
		}
	}
	return result;
}

void Game_Player::Refresh() {
	Game_Actor* actor;

	if (Main_Data::game_party->GetActors().empty()) {
		SetSpriteName("");
		SetSpriteIndex(0);
		return;
	}

	actor = Main_Data::game_party->GetActors()[0];

	SetSpriteName(actor->GetSpriteName());
	SetSpriteIndex(actor->GetSpriteIndex());

	if (data()->aboard)
		GetVehicle()->SyncWithPlayer();
}

bool Game_Player::GetOnOffVehicle() {
	if (IsBoardingOrUnboarding()) {
		return false;
	}

	if (InVehicle())
		return GetOffVehicle();
	return GetOnVehicle();
}

bool Game_Player::GetOnVehicle() {
	int front_x = Game_Map::XwithDirection(GetX(), GetDirection());
	int front_y = Game_Map::YwithDirection(GetY(), GetDirection());
	Game_Vehicle::Type type;

	if (Game_Map::GetVehicle(Game_Vehicle::Airship)->IsInPosition(GetX(), GetY()))
		type = Game_Vehicle::Airship;
	else if (Game_Map::GetVehicle(Game_Vehicle::Ship)->IsInPosition(front_x, front_y))
		type = Game_Vehicle::Ship;
	else if (Game_Map::GetVehicle(Game_Vehicle::Boat)->IsInPosition(front_x, front_y))
		type = Game_Vehicle::Boat;
	else
		return false;

	auto* vehicle = Game_Map::GetVehicle(type);

	if (vehicle->IsAscendingOrDescending()) {
		return false;
	}

	if (type == Game_Vehicle::Airship && (IsMoving() || IsJumping())) {
		return false;
	}

	if (type != Game_Vehicle::Airship && !Game_Map::CanEmbarkShip(*this, front_x, front_y)) {
		return false;
	}

	data()->vehicle = type;
	data()->preboard_move_speed = GetMoveSpeed();
	if (type != Game_Vehicle::Airship) {
		data()->boarding = true;
		if (!IsMoving() && !IsJumping()) {
			if (!GetThrough()) {
				SetThrough(true);
				MoveForward();
				SetThrough(false);
			} else {
				MoveForward();
			}
		}
	} else {
		data()->aboard = true;
		if (vehicle->IsMoveRouteOverwritten()) {
			vehicle->CancelMoveRoute();
		}
		SetMoveSpeed(vehicle->GetMoveSpeed());
		SetDirection(RPG::EventPage::Direction_left);
		// Note: RPG_RT ignores the lock_facing flag here!
		SetSpriteDirection(RPG::EventPage::Direction_left);
		vehicle->SetX(GetX());
		vehicle->SetY(GetY());
	}

	Main_Data::game_data.system.before_vehicle_music = Game_System::GetCurrentBGM();
	GetVehicle()->GetOn();
	return true;
}

bool Game_Player::GetOffVehicle() {
	if (!InAirship()) {
		int front_x = Game_Map::XwithDirection(GetX(), GetDirection());
		int front_y = Game_Map::YwithDirection(GetY(), GetDirection());
		if (!Game_Map::CanDisembarkShip(*this, front_x, front_y)) {
			return false;
		}
	}
	auto* vehicle = GetVehicle();

	if (vehicle->IsAscendingOrDescending()) {
		return false;
	}

	vehicle->GetOff();
	return true;
}

bool Game_Player::IsStopping() const {
	// Prevent MoveRoute execution while the airship is ascending/descending (Issue #1268)
	if (InAirship() && !GetVehicle()->IsMovable())
		return false;
	return Game_Character::IsStopping();
}

bool Game_Player::IsMovable() const {
	if (IsMoving() || IsJumping())
		return false;
	if (Graphics::IsTransitionPending())
		return false;
	if (IsMoveRouteOverwritten())
		return false;
	if (data()->boarding || data()->unboarding)
		return false;
	if (Game_Message::message_waiting)
		return false;
	if (InAirship() && !GetVehicle()->IsMovable())
		return false;
    return true;
}

bool Game_Player::InVehicle() const {
	return data()->vehicle > 0;
}

bool Game_Player::InAirship() const {
	return data()->vehicle == Game_Vehicle::Airship;
}

Game_Vehicle* Game_Player::GetVehicle() const {
	return Game_Map::GetVehicle((Game_Vehicle::Type) data()->vehicle);
}

void Game_Player::BeginMove() {
	int terrain_id = Game_Map::GetTerrainTag(GetX(), GetY());
	const RPG::Terrain* terrain = ReaderUtil::GetElement(Data::terrains, terrain_id);
	bool red_flash = false;

	if (terrain) {
		if (!InAirship()) {
			if (!terrain->on_damage_se || (terrain->on_damage_se && (terrain->damage > 0))) {
				Game_System::SePlay(terrain->footstep);
			}
			if (terrain->damage > 0) {
				for (auto hero : Main_Data::game_party->GetActors()) {
					if (!hero->PreventsTerrainDamage()) {
						red_flash = true;
						hero->ChangeHp(-std::max<int>(0, std::min<int>(terrain->damage, hero->GetHp() - 1)));
					}
				}
			}
		}
	} else {
		Output::Warning("Player BeginMove: Invalid terrain ID %d at (%d, %d)", terrain_id, GetX(), GetY());
	}

	red_flash = red_flash || Main_Data::game_party->ApplyStateDamage();

	if (red_flash) {
		Main_Data::game_screen->FlashOnce(31, 10, 10, 19, 6);
	}
}

void Game_Player::Unboard() {
	data()->aboard = false;
	SetMoveSpeed(data()->preboard_move_speed);

	Game_System::BgmPlay(Main_Data::game_data.system.before_vehicle_music);
}

bool Game_Player::IsAboard() const {
	return data()->aboard;
}

bool Game_Player::IsBoardingOrUnboarding() const {
	return data()->boarding || data()->unboarding;
}

void Game_Player::UnboardingFinished() {
	Unboard();
	if (InAirship()) {
		SetDirection(RPG::EventPage::Direction_down);
		// Note: RPG_RT ignores the lock_facing flag here!
		SetSpriteDirection(RPG::EventPage::Direction_down);
	} else {
		data()->unboarding = true;
		if (!IsMoving() && !IsJumping()) {
			if (!GetThrough()) {
				SetThrough(true);
				MoveForward();
				SetThrough(false);
			} else {
				MoveForward();
			}
		}
	}
	data()->vehicle = Game_Vehicle::None;
}

int Game_Player::GetVehicleType() const {
	return data()->vehicle;
}
