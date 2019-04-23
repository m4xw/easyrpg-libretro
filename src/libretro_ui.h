/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EP_LIBRETRO_UI_H
#define EP_LIBRETRO_UI_H

#ifdef USE_LIBRETRO

// Headers
#include "audio_libretro.h"
#include "baseui.h"
#include "color.h"
#include "rect.h"
#include "system.h"

#include "libretro.h"

/**
 * LibretroUi class.
 */
class LibretroUi : public BaseUi {
public:
	/**
	 * Constructor.
	 *
	 * @param width window client width.
	 * @param height window client height.
	 * @param title window title.
	 * @param fullscreen start in fullscreen flag.
	 */
	LibretroUi(int width, int height);

	/**
	 * Inherited from BaseUi.
	 */
	/** @{ */
	void BeginDisplayModeChange() override;
	void EndDisplayModeChange() override;
	void Resize(long width, long height) override;
	void ToggleFullscreen() override;
	void ToggleZoom() override;
	void UpdateDisplay() override;
	void SetTitle(const std::string &title) override;
	bool ShowCursor(bool flag) override;

	void ProcessEvents() override;

	bool IsFullscreen() override;

	uint32_t GetTicks() const override;
	void Sleep(uint32_t time_milli) override;

#ifdef SUPPORT_AUDIO
	AudioInterface& GetAudio() override;
	std::unique_ptr<AudioInterface> audio_;
#endif
	/** @} */

public:
	static void SetRetroVideoCallback(retro_video_refresh_t cb);
	static void SetRetroInputStateCallback(retro_input_state_t cb);
	static retro_usec_t time_in_microseconds;
private:
	static retro_video_refresh_t UpdateWindow;
	static retro_input_state_t CheckInputState;

};

#endif
#endif
