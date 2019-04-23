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

#ifndef LCF_RPG_SWITCH_H
#define LCF_RPG_SWITCH_H

// Headers
#include <string>

/**
 * RPG::Switch class.
 */
namespace RPG {
	class Switch {
	public:
		int ID = 0;
		std::string name;
	};

	inline bool operator==(const Switch& l, const Switch& r) {
		return l.name == r.name;
	}

	inline bool operator!=(const Switch& l, const Switch& r) {
		return !(l == r);
	}
}

#endif
