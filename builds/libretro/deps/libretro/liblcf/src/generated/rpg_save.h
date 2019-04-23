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

#ifndef LCF_RPG_SAVE_H
#define LCF_RPG_SAVE_H

// Headers
#include <vector>
#include "rpg_saveactor.h"
#include "rpg_savecommonevent.h"
#include "rpg_saveeventdata.h"
#include "rpg_saveinventory.h"
#include "rpg_savemapinfo.h"
#include "rpg_savepanorama.h"
#include "rpg_savepartylocation.h"
#include "rpg_savepicture.h"
#include "rpg_savescreen.h"
#include "rpg_savesystem.h"
#include "rpg_savetarget.h"
#include "rpg_savetitle.h"
#include "rpg_savevehiclelocation.h"

/**
 * RPG::Save class.
 */
namespace RPG {
	class Save {
	public:
		void Setup();
		SaveTitle title;
		SaveSystem system;
		SaveScreen screen;
		std::vector<SavePicture> pictures;
		SavePartyLocation party_location;
		SaveVehicleLocation boat_location;
		SaveVehicleLocation ship_location;
		SaveVehicleLocation airship_location;
		std::vector<SaveActor> actors;
		SaveInventory inventory;
		std::vector<SaveTarget> targets;
		SaveMapInfo map_info;
		SavePanorama panorama;
		SaveEventData events;
		std::vector<SaveCommonEvent> common_events;
	};
}

#endif
