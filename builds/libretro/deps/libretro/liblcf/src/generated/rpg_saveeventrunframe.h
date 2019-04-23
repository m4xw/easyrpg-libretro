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

#ifndef LCF_RPG_SAVEEVENTRUNFRAME_H
#define LCF_RPG_SAVEEVENTRUNFRAME_H

// Headers
#include <stdint.h>
#include <vector>
#include "rpg_eventcommand.h"

/**
 * RPG::SaveEventRunFrame class.
 */
namespace RPG {
	class SaveEventRunFrame {
	public:
		int ID = 0;
		int32_t commands_size = 0;
		std::vector<EventCommand> commands;
		int32_t current_command = 0;
		int32_t event_id = 0;
		bool triggered_by_decision_key = false;
		std::vector<uint8_t> subcommand_path;
	};

	inline bool operator==(const SaveEventRunFrame& l, const SaveEventRunFrame& r) {
		return l.commands_size == r.commands_size
		&& l.commands == r.commands
		&& l.current_command == r.current_command
		&& l.event_id == r.event_id
		&& l.triggered_by_decision_key == r.triggered_by_decision_key
		&& l.subcommand_path == r.subcommand_path
		&& l.subcommand_path == r.subcommand_path;
	}

	inline bool operator!=(const SaveEventRunFrame& l, const SaveEventRunFrame& r) {
		return !(l == r);
	}
}

#endif
