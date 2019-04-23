/* !!!! GENERATED FILE - DO NOT EDIT !!!!
 * --------------------------------------
 *
 * This file is part of liblcf. Copyright (c) 2018 liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#ifndef LCF_RPG_ENEMYACTION_H
#define LCF_RPG_ENEMYACTION_H

// Headers
#include <stdint.h>
#include "enum_tags.h"

/**
 * RPG::EnemyAction class.
 */
namespace RPG {
	class EnemyAction {
	public:
		enum Kind {
			Kind_basic = 0,
			Kind_skill = 1,
			Kind_transformation = 2
		};
		static constexpr auto kKindTags = makeEnumTags<Kind>(
			"basic",
			"skill",
			"transformation"
		);
		enum Basic {
			Basic_attack = 0,
			Basic_dual_attack = 1,
			Basic_defense = 2,
			Basic_observe = 3,
			Basic_charge = 4,
			Basic_autodestruction = 5,
			Basic_escape = 6,
			Basic_nothing = 7
		};
		static constexpr auto kBasicTags = makeEnumTags<Basic>(
			"attack",
			"dual_attack",
			"defense",
			"observe",
			"charge",
			"autodestruction",
			"escape",
			"nothing"
		);
		enum ConditionType {
			ConditionType_always = 0,
			ConditionType_switch = 1,
			ConditionType_turn = 2,
			ConditionType_actors = 3,
			ConditionType_hp = 4,
			ConditionType_sp = 5,
			ConditionType_party_lvl = 6,
			ConditionType_party_fatigue = 7
		};
		static constexpr auto kConditionTypeTags = makeEnumTags<ConditionType>(
			"always",
			"switch",
			"turn",
			"actors",
			"hp",
			"sp",
			"party_lvl",
			"party_fatigue"
		);

		int ID = 0;
		int32_t kind = 0;
		int32_t basic = 1;
		int32_t skill_id = 1;
		int32_t enemy_id = 1;
		int32_t condition_type = 0;
		int32_t condition_param1 = 0;
		int32_t condition_param2 = 0;
		int32_t switch_id = 1;
		bool switch_on = false;
		int32_t switch_on_id = 1;
		bool switch_off = false;
		int32_t switch_off_id = 1;
		int32_t rating = 50;
	};
}

#endif
