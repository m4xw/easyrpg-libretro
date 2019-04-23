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

// Headers
#include "ldb_reader.h"
#include "ldb_chunks.h"
#include "reader_struct_impl.h"

// Read TroopPageCondition.

template <>
char const* const Struct<RPG::TroopPageCondition>::name = "TroopPageCondition";

template <>
Field<RPG::TroopPageCondition> const* Struct<RPG::TroopPageCondition>::fields[] = {
	new TypedField<RPG::TroopPageCondition, RPG::TroopPageCondition::Flags>(
		&RPG::TroopPageCondition::flags,
		LDB_Reader::ChunkTroopPageCondition::flags,
		"flags",
		1,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::switch_a_id,
		LDB_Reader::ChunkTroopPageCondition::switch_a_id,
		"switch_a_id",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::switch_b_id,
		LDB_Reader::ChunkTroopPageCondition::switch_b_id,
		"switch_b_id",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::variable_id,
		LDB_Reader::ChunkTroopPageCondition::variable_id,
		"variable_id",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::variable_value,
		LDB_Reader::ChunkTroopPageCondition::variable_value,
		"variable_value",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::turn_a,
		LDB_Reader::ChunkTroopPageCondition::turn_a,
		"turn_a",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::turn_b,
		LDB_Reader::ChunkTroopPageCondition::turn_b,
		"turn_b",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::fatigue_min,
		LDB_Reader::ChunkTroopPageCondition::fatigue_min,
		"fatigue_min",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::fatigue_max,
		LDB_Reader::ChunkTroopPageCondition::fatigue_max,
		"fatigue_max",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::enemy_id,
		LDB_Reader::ChunkTroopPageCondition::enemy_id,
		"enemy_id",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::enemy_hp_min,
		LDB_Reader::ChunkTroopPageCondition::enemy_hp_min,
		"enemy_hp_min",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::enemy_hp_max,
		LDB_Reader::ChunkTroopPageCondition::enemy_hp_max,
		"enemy_hp_max",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::actor_id,
		LDB_Reader::ChunkTroopPageCondition::actor_id,
		"actor_id",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::actor_hp_min,
		LDB_Reader::ChunkTroopPageCondition::actor_hp_min,
		"actor_hp_min",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::actor_hp_max,
		LDB_Reader::ChunkTroopPageCondition::actor_hp_max,
		"actor_hp_max",
		0,
		0
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::turn_enemy_id,
		LDB_Reader::ChunkTroopPageCondition::turn_enemy_id,
		"turn_enemy_id",
		0,
		1
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::turn_enemy_a,
		LDB_Reader::ChunkTroopPageCondition::turn_enemy_a,
		"turn_enemy_a",
		0,
		1
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::turn_enemy_b,
		LDB_Reader::ChunkTroopPageCondition::turn_enemy_b,
		"turn_enemy_b",
		0,
		1
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::turn_actor_id,
		LDB_Reader::ChunkTroopPageCondition::turn_actor_id,
		"turn_actor_id",
		0,
		1
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::turn_actor_a,
		LDB_Reader::ChunkTroopPageCondition::turn_actor_a,
		"turn_actor_a",
		0,
		1
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::turn_actor_b,
		LDB_Reader::ChunkTroopPageCondition::turn_actor_b,
		"turn_actor_b",
		0,
		1
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::command_actor_id,
		LDB_Reader::ChunkTroopPageCondition::command_actor_id,
		"command_actor_id",
		0,
		1
	),
	new TypedField<RPG::TroopPageCondition, int32_t>(
		&RPG::TroopPageCondition::command_id,
		LDB_Reader::ChunkTroopPageCondition::command_id,
		"command_id",
		0,
		1
	),
	NULL
};

template class Struct<RPG::TroopPageCondition>;
