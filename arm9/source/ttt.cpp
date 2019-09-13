/*-------------------------------------------------------------------------
Loading Tic-Tac-Toe
--------------------------------------------------------------------------*/

#include <nds.h>
#include <stdio.h>

extern int screenMode;
extern void fadeOut(void);

static bool drawnStuff = false;

static int ttt_selected[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};	// 0 = Blank, 1 = N, 2 = T
static int ttt_highlighted = 0;

static u16 ttt_rectColor[9] = {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF};

static u16 ttt_selColor = 0x03A0;

//static bool ttt_keypressed = false;

static int ttt_drawXpos = 0;
static int ttt_drawYpos = 0;

static void ttt_drawN(void) {
	// 1st h line
	for (int y = ttt_drawYpos+0; y <= ttt_drawYpos+39; y++) {
		for (int k = ttt_drawXpos+0; k <= ttt_drawXpos+7; k++) {
			VRAM_A[y*256+k] = 0x0000;
		}
	}
	// 2nd h line
	for (int y = ttt_drawYpos+8; y <= ttt_drawYpos+15; y++) {
		for (int k = ttt_drawXpos+8; k <= ttt_drawXpos+15; k++) {
			VRAM_A[y*256+k] = 0x0000;
		}
	}
	// 3rd h line
	for (int y = ttt_drawYpos+16; y <= ttt_drawYpos+23; y++) {
		for (int k = ttt_drawXpos+16; k <= ttt_drawXpos+23; k++) {
			VRAM_A[y*256+k] = 0x0000;
		}
	}
	// 4th h line
	for (int y = ttt_drawYpos+24; y <= ttt_drawYpos+31; y++) {
		for (int k = ttt_drawXpos+24; k <= ttt_drawXpos+31; k++) {
			VRAM_A[y*256+k] = 0x0000;
		}
	}
	// 5th h line
	for (int y = ttt_drawYpos+0; y <= ttt_drawYpos+39; y++) {
		for (int k = ttt_drawXpos+32; k <= ttt_drawXpos+39; k++) {
			VRAM_A[y*256+k] = 0x0000;
		}
	}
}

static void ttt_drawT(void) {
	// Top
	for (int y = ttt_drawYpos+0; y <= ttt_drawYpos+7; y++) {
		for (int k = ttt_drawXpos+0; k <= ttt_drawXpos+39; k++) {
			VRAM_A[y*256+k] = 0x0000;
		}
	}
	// Bottom
	for (int y = ttt_drawYpos+8; y <= ttt_drawYpos+39; y++) {
		for (int k = ttt_drawXpos+16; k <= ttt_drawXpos+23; k++) {
			VRAM_A[y*256+k] = 0x0000;
		}
	}
}

void ticTacToe(int pressed) {
	if (!drawnStuff) {
		// Draw white BG
		for (int i = 0; i < 256*192; i++) {
			VRAM_A[i] = 0x7FFF;
		}

		// Draw top of v line 1
		for (int y = 56; y <= 59; y++) {
			for (int k = 0; k <= 256; k++) {
				VRAM_A[y*256+k] = 0x4631;
			}
		}

		// Draw v line 1
		for (int y = 60; y <= 67; y++) {
			for (int k = 0; k <= 256; k++) {
				VRAM_A[y*256+k] = 0x0000;
			}
		}

		// Draw v line 2
		for (int y = 124; y <= 131; y++) {
			for (int k = 0; k <= 256; k++) {
				VRAM_A[y*256+k] = 0x0000;
			}
		}

		// Draw bottom of v line 2
		for (int y = 132; y <= 135; y++) {
			for (int k = 0; k <= 256; k++) {
				VRAM_A[y*256+k] = 0x4631;
			}
		}

		// Draw h line 1
		for (int y = 0; y <= 191; y++) {
			for (int k = 80; k <= 87; k++) {
				VRAM_A[y*256+k] = 0x0000;
			}
		}

		// Draw h line 2
		for (int y = 0; y <= 191; y++) {
			for (int k = 168; k <= 175; k++) {
				VRAM_A[y*256+k] = 0x0000;
			}
		}

		drawnStuff = true;
	}
	
	//if (arm9_errorColor) ttt_selColor = 0x001B;
	
	// Draw highlighter, vertical line 1
	for (int e = 0; e <= 2; e++) {
		// Vertical parts
		for (int y = 0; y <= 7; y++) {
			for (int k = 0; k <= 79; k++) {
				VRAM_A[y*256+k+e*88] = ttt_rectColor[e];
			}
		}
		for (int y = 48; y <= 55; y++) {
			for (int k = 0; k <= 79; k++) {
				VRAM_A[y*256+k+e*88] = ttt_rectColor[e];
			}
		}
		// Horizontal parts
		for (int i = 0; i <= 72; i += 72) {
			for (int y = 8; y <= 47; y++) {
				for (int k = 0; k <= 7; k++) {
					VRAM_A[y*256+k+i+e*88] = ttt_rectColor[e];
				}
			}
		}
	}

	// Draw highlighter, vertical line 2
	for (int e = 0; e <= 2; e++) {
		// Vertical parts
		for (int y = 68; y <= 75; y++) {
			for (int k = 0; k <= 79; k++) {
				VRAM_A[y*256+k+e*88] = ttt_rectColor[e+3];
			}
		}
		for (int y = 116; y <= 123; y++) {
			for (int k = 0; k <= 79; k++) {
				VRAM_A[y*256+k+e*88] = ttt_rectColor[e+3];
			}
		}
		// Horizontal parts
		for (int i = 0; i <= 72; i += 72) {
			for (int y = 76; y <= 115; y++) {
				for (int k = 0; k <= 7; k++) {
					VRAM_A[y*256+k+i+e*88] = ttt_rectColor[e+3];
				}
			}
		}
	}

	// Draw highlighter, vertical line 3
	for (int e = 0; e <= 2; e++) {
		// Vertical parts
		for (int y = 136; y <= 143; y++) {
			for (int k = 0; k <= 79; k++) {
				VRAM_A[y*256+k+e*88] = ttt_rectColor[e+6];
			}
		}
		for (int y = 184; y <= 191; y++) {
			for (int k = 0; k <= 79; k++) {
				VRAM_A[y*256+k+e*88] = ttt_rectColor[e+6];
			}
		}
		// Horizontal parts
		for (int i = 0; i <= 72; i += 72) {
			for (int y = 144; y <= 183; y++) {
				for (int k = 0; k <= 7; k++) {
					VRAM_A[y*256+k+i+e*88] = ttt_rectColor[e+6];
				}
			}
		}
	}

	// Control highlighter
	if (pressed & KEY_UP) {
		ttt_highlighted -= 3;
		if (ttt_highlighted < 0) ttt_highlighted = 0;
	}
	if (pressed & KEY_DOWN) {
		ttt_highlighted += 3;
		if (ttt_highlighted > 8) ttt_highlighted = 8;
	}
	if (pressed & KEY_LEFT) {
		ttt_highlighted--;
		if (ttt_highlighted < 0) ttt_highlighted = 0;
	}
	if (pressed & KEY_RIGHT) {
		ttt_highlighted++;
		if (ttt_highlighted > 8) ttt_highlighted = 8;
	}

	if (pressed & KEY_Y) {
		if (ttt_selected[ttt_highlighted] == 0){
			ttt_selected[ttt_highlighted] = 1;	// N
			// Set X position
			switch (ttt_highlighted) {
				case 0:
				case 3:
				case 6:
				default:
					ttt_drawXpos = 20;
					break;
				case 1:
				case 4:
				case 7:
					ttt_drawXpos = 108;
					break;
				case 2:
				case 5:
				case 8:
					ttt_drawXpos = 196;
					break;
			}
			// Set Y position
			switch (ttt_highlighted) {
				case 0:
				case 1:
				case 2:
				default:
					ttt_drawYpos = 8;
					break;
				case 3:
				case 4:
				case 5:
					ttt_drawYpos = 76;
					break;
				case 6:
				case 7:
				case 8:
					ttt_drawYpos = 144;
					break;
			}
			ttt_drawN();
		}
	}
	if (pressed & KEY_A) {
		if (ttt_selected[ttt_highlighted] == 0){
			ttt_selected[ttt_highlighted] = 2;	// T
			// Set X position
			switch (ttt_highlighted) {
				case 0:
				case 3:
				case 6:
				default:
					ttt_drawXpos = 20;
					break;
				case 1:
				case 4:
				case 7:
					ttt_drawXpos = 108;
					break;
				case 2:
				case 5:
				case 8:
					ttt_drawXpos = 196;
					break;
			}
			// Set Y position
			switch (ttt_highlighted) {
				case 0:
				case 1:
				case 2:
				default:
					ttt_drawYpos = 8;
					break;
				case 3:
				case 4:
				case 5:
					ttt_drawYpos = 76;
					break;
				case 6:
				case 7:
				case 8:
					ttt_drawYpos = 144;
					break;
			}
			ttt_drawT();
		}
	}

	if (pressed & KEY_X) {
		// Clear all marks
		drawnStuff = false;
		for (int i = 0; i < 9; i++) ttt_selected[i] = 0;
	}

	if (pressed & KEY_SELECT) {
		fadeOut();
		screenMode++;
		ttt_highlighted = 0;

		// Clear all marks
		drawnStuff = false;
		for (int i = 0; i < 9; i++) ttt_selected[i] = 0;
	}

	if (ttt_highlighted == 0) {
		ttt_rectColor[0] = ttt_selColor;
	} else {
		ttt_rectColor[0] = 0x7FFF;
	}
	if (ttt_highlighted == 1) {
		ttt_rectColor[1] = ttt_selColor;
	} else {
		ttt_rectColor[1] = 0x7FFF;
	}
	if (ttt_highlighted == 2) {
		ttt_rectColor[2] = ttt_selColor;
	} else {
		ttt_rectColor[2] = 0x7FFF;
	}
	if (ttt_highlighted == 3) {
		ttt_rectColor[3] = ttt_selColor;
	} else {
		ttt_rectColor[3] = 0x7FFF;
	}
	if (ttt_highlighted == 4) {
		ttt_rectColor[4] = ttt_selColor;
	} else {
		ttt_rectColor[4] = 0x7FFF;
	}
	if (ttt_highlighted == 5) {
		ttt_rectColor[5] = ttt_selColor;
	} else {
		ttt_rectColor[5] = 0x7FFF;
	}
	if (ttt_highlighted == 6) {
		ttt_rectColor[6] = ttt_selColor;
	} else {
		ttt_rectColor[6] = 0x7FFF;
	}
	if (ttt_highlighted == 7) {
		ttt_rectColor[7] = ttt_selColor;
	} else {
		ttt_rectColor[7] = 0x7FFF;
	}
	if (ttt_highlighted == 8) {
		ttt_rectColor[8] = ttt_selColor;
	} else {
		ttt_rectColor[8] = 0x7FFF;
	}
	
	swiWaitForVBlank();
}