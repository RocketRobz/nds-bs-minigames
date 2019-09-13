/*-----------------------------------------------------------------
 Copyright (C) 2005 - 2013
	Michael "Chishm" Chisholm
	Dave "WinterMute" Murphy
	Claudio "sverx"

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
//#include <fat.h>
#include <sys/stat.h>
#include <limits.h>

#include <string.h>
#include <unistd.h>

#include "logo_rocketrobz.h"
#include "title.h"

int screenBrightness = 25;

int screenMode = 0;
bool gamePaused = false;

extern void pongGame(int pressed);
extern void ticTacToe(int pressed);

void SetBrightness(u8 screen, s8 bright) {
	u16 mode = 1 << 14;

	if (bright < 0) {
		mode = 2 << 14;
		bright = -bright;
	}
	if (bright > 31) {
		bright = 31;
	}
	*(u16*)(0x0400006C + (0x1000 * screen)) = bright + mode;
}

void fadeIn(void) {
	for (screenBrightness = 25; screenBrightness >= 0; screenBrightness--) {
		SetBrightness(0, screenBrightness);
		SetBrightness(1, screenBrightness);
		swiWaitForVBlank();
	}
}

void fadeOut(void) {
	for (screenBrightness = 0; screenBrightness <= 25; screenBrightness++) {
		SetBrightness(0, screenBrightness);
		SetBrightness(1, screenBrightness);
		swiWaitForVBlank();
	}
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	defaultExceptionHandler();

	*(u16*)0x0400006C |= BIT(14);
	*(u16*)0x0400006C &= BIT(15);
	SetBrightness(0, 31);
	SetBrightness(1, 31);

	// initialize video mode
	videoSetMode(MODE_4_2D);

	// initialize VRAM banks
	vramSetPrimaryBanks(VRAM_A_MAIN_BG,
	                    VRAM_B_MAIN_SPRITE,
	                    VRAM_C_LCD,
	                    VRAM_D_LCD);

	// initialize bg3 as a bmp background
	int bg3 = bgInit(3, BgType_Bmp16,    BgSize_B16_256x256, 1, 0);

	// initialize rotate, scale, and scroll
	bgSetRotateScale(bg3, 0, 1<<8, 1<<8);
	bgSetScroll(bg3, 0, 0);

	// load compressed bitmap into bg3
	decompress(logo_rocketrobzBitmap, bgGetGfxPtr(bg3), LZ77Vram);

	// apply the bg changes
	bgUpdate();

	fadeIn();
	for (int i = 0; i < 60*2; i++) {
		swiWaitForVBlank();
	}
	fadeOut();

	// load compressed bitmap into bg3
	decompress(titleBitmap, bgGetGfxPtr(bg3), LZ77Vram);

	// apply the bg changes
	bgUpdate();

	fadeIn();
	while(1) {
		scanKeys();
		int pressed = keysDown();

		if (pressed & KEY_START) {
			break;
		}

		if (pressed & KEY_B) {
			fadeOut();
			return 0;
		}
	}
	fadeOut();

	REG_POWERCNT = (u16)(POWER_LCD | POWER_2D_A | POWER_SWAP_LCDS);
	REG_DISPCNT = MODE_FB0;
	VRAM_A_CR = VRAM_ENABLE;

	while(1) {
		scanKeys();
		int pressed = keysDown();

		if (!gamePaused) {
			switch (screenMode) {
				case 0:
				default:
					pongGame(pressed);
					break;
				case 1:
					ticTacToe(pressed);
					break;
			}
		}

		if (pressed & KEY_START) gamePaused = !gamePaused;

		if (pressed & KEY_B) {
			fadeOut();
			break;
		}

		if (screenMode < 0) screenMode = 1;
		else if (screenMode > 1) screenMode = 0;

		screenBrightness--;
		if (screenBrightness < 0) screenBrightness = 0;
		SetBrightness(0, screenBrightness);
		SetBrightness(1, screenBrightness);
	}

	return 0;
}
