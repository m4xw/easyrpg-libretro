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

#ifndef LCF_RPG_SAVECOMMONEVENT_H
#define LCF_RPG_SAVECOMMONEVENT_H

// Headers
#include "rpg_saveeventexecstate.h"

/**
 * RPG::SaveCommonEvent class.
 */
namespace RPG {
	class SaveCommonEvent {
	public:
		int ID = 0;
		SaveEventExecState parallel_event_execstate;
	};

	inline bool operator==(const SaveCommonEvent& l, const SaveCommonEvent& r) {
		return l.parallel_event_execstate == r.parallel_event_execstate;
	}

	inline bool operator!=(const SaveCommonEvent& l, const SaveCommonEvent& r) {
		return !(l == r);
	}
}

#endif
