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

#ifndef LCF_RPG_ATTRIBUTE_H
#define LCF_RPG_ATTRIBUTE_H

// Headers
#include <stdint.h>
#include <string>
#include "enum_tags.h"

/**
 * RPG::Attribute class.
 */
namespace RPG {
	class Attribute {
	public:
		enum Type {
			Type_physical = 0,
			Type_magical = 1
		};
		static constexpr auto kTypeTags = makeEnumTags<Type>(
			"physical",
			"magical"
		);

		int ID = 0;
		std::string name;
		int32_t type = 0;
		int32_t a_rate = 300;
		int32_t b_rate = 200;
		int32_t c_rate = 100;
		int32_t d_rate = 50;
		int32_t e_rate = 0;
	};
}

#endif
