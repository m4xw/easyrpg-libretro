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

// Read BattlerAnimation.

template <>
char const* const Struct<RPG::BattlerAnimation>::name = "BattlerAnimation";

template <>
Field<RPG::BattlerAnimation> const* Struct<RPG::BattlerAnimation>::fields[] = {
	new TypedField<RPG::BattlerAnimation, std::string>(
		&RPG::BattlerAnimation::name,
		LDB_Reader::ChunkBattlerAnimation::name,
		"name",
		0,
		0
	),
	new TypedField<RPG::BattlerAnimation, int32_t>(
		&RPG::BattlerAnimation::speed,
		LDB_Reader::ChunkBattlerAnimation::speed,
		"speed",
		0,
		0
	),
	new TypedField<RPG::BattlerAnimation, std::vector<RPG::BattlerAnimationExtension>>(
		&RPG::BattlerAnimation::base_data,
		LDB_Reader::ChunkBattlerAnimation::base_data,
		"base_data",
		1,
		0
	),
	new TypedField<RPG::BattlerAnimation, std::vector<RPG::BattlerAnimationExtension>>(
		&RPG::BattlerAnimation::weapon_data,
		LDB_Reader::ChunkBattlerAnimation::weapon_data,
		"weapon_data",
		1,
		0
	),
	NULL
};

template class Struct<RPG::BattlerAnimation>;
