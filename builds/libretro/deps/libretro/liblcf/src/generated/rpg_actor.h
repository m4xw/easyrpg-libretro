/* !!!! GENERATED FILE - DO NOT EDIT !!!!
 * --------------------------------------
 *
 * This file is part of liblcf. Copyright (c) 2019 liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#ifndef LCF_RPG_ACTOR_H
#define LCF_RPG_ACTOR_H

// Headers
#include <stdint.h>
#include <string>
#include <vector>
#include "rpg_equipment.h"
#include "rpg_learning.h"
#include "rpg_parameters.h"

/**
 * RPG::Actor class.
 */
namespace RPG {
	class Actor {
	public:
		void Setup();
		int ID = 0;
		std::string name;
		std::string title;
		std::string character_name;
		int32_t character_index = 0;
		bool transparent = false;
		int32_t initial_level = 1;
		int32_t final_level = -1;
		bool critical_hit = true;
		int32_t critical_hit_chance = 30;
		std::string face_name;
		int32_t face_index = 0;
		bool two_weapon = false;
		bool lock_equipment = false;
		bool auto_battle = false;
		bool super_guard = false;
		Parameters parameters;
		int32_t exp_base = -1;
		int32_t exp_inflation = -1;
		int32_t exp_correction = 0;
		Equipment initial_equipment;
		int32_t unarmed_animation = 1;
		int32_t class_id = 0;
		int32_t battle_x = 220;
		int32_t battle_y = 120;
		int32_t battler_animation = 1;
		std::vector<Learning> skills;
		bool rename_skill = false;
		std::string skill_name;
		std::vector<uint8_t> state_ranks;
		std::vector<uint8_t> attribute_ranks;
		std::vector<int32_t> battle_commands;
	};

	inline bool operator==(const Actor& l, const Actor& r) {
		return l.name == r.name
		&& l.title == r.title
		&& l.character_name == r.character_name
		&& l.character_index == r.character_index
		&& l.transparent == r.transparent
		&& l.initial_level == r.initial_level
		&& l.final_level == r.final_level
		&& l.critical_hit == r.critical_hit
		&& l.critical_hit_chance == r.critical_hit_chance
		&& l.face_name == r.face_name
		&& l.face_index == r.face_index
		&& l.two_weapon == r.two_weapon
		&& l.lock_equipment == r.lock_equipment
		&& l.auto_battle == r.auto_battle
		&& l.super_guard == r.super_guard
		&& l.parameters == r.parameters
		&& l.exp_base == r.exp_base
		&& l.exp_inflation == r.exp_inflation
		&& l.exp_correction == r.exp_correction
		&& l.initial_equipment == r.initial_equipment
		&& l.unarmed_animation == r.unarmed_animation
		&& l.class_id == r.class_id
		&& l.battle_x == r.battle_x
		&& l.battle_y == r.battle_y
		&& l.battler_animation == r.battler_animation
		&& l.skills == r.skills
		&& l.rename_skill == r.rename_skill
		&& l.skill_name == r.skill_name
		&& l.state_ranks == r.state_ranks
		&& l.state_ranks == r.state_ranks
		&& l.attribute_ranks == r.attribute_ranks
		&& l.attribute_ranks == r.attribute_ranks
		&& l.battle_commands == r.battle_commands;
	}

	inline bool operator!=(const Actor& l, const Actor& r) {
		return !(l == r);
	}
}

#endif
