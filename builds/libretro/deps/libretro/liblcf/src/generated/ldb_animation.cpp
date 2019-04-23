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

// Read Animation.

template <>
char const* const Struct<RPG::Animation>::name = "Animation";

template <>
Field<RPG::Animation> const* Struct<RPG::Animation>::fields[] = {
	new TypedField<RPG::Animation, std::string>(
		&RPG::Animation::name,
		LDB_Reader::ChunkAnimation::name,
		"name",
		0,
		0
	),
	new TypedField<RPG::Animation, std::string>(
		&RPG::Animation::animation_name,
		LDB_Reader::ChunkAnimation::animation_name,
		"animation_name",
		0,
		0
	),
	new TypedField<RPG::Animation, bool>(
		&RPG::Animation::large,
		LDB_Reader::ChunkAnimation::large,
		"large",
		0,
		0
	),
	new TypedField<RPG::Animation, std::vector<RPG::AnimationTiming>>(
		&RPG::Animation::timings,
		LDB_Reader::ChunkAnimation::timings,
		"timings",
		1,
		0
	),
	new TypedField<RPG::Animation, int32_t>(
		&RPG::Animation::scope,
		LDB_Reader::ChunkAnimation::scope,
		"scope",
		1,
		0
	),
	new TypedField<RPG::Animation, int32_t>(
		&RPG::Animation::position,
		LDB_Reader::ChunkAnimation::position,
		"position",
		1,
		0
	),
	new TypedField<RPG::Animation, std::vector<RPG::AnimationFrame>>(
		&RPG::Animation::frames,
		LDB_Reader::ChunkAnimation::frames,
		"frames",
		1,
		0
	),
	NULL
};

template class Struct<RPG::Animation>;
