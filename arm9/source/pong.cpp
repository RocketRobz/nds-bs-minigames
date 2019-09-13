/*-------------------------------------------------------------------------
Loading pong
--------------------------------------------------------------------------*/

#include <nds.h>
#include <stdio.h>

extern int screenMode;

static bool drawnStuff = false;

static int leftpaddle_yPos = 76;
static int rightpaddle_yPos = 76;
static int ball_xPos = 16;
static int ball_yPos = 72;
static int ball_ySpeed = 2;
static int ball_moveUD = false;	// false = up, true = down
static int ball_moveLR = true;	// false = left, true = right

static u16 pong_color = 0x0000;

static void restartGame(void) {
	leftpaddle_yPos = 76;
	rightpaddle_yPos = 76;
	ball_xPos = 16;
	ball_yPos = 64;
	ball_ySpeed = 2;
	ball_moveUD = false;	// false = up, true = down
	ball_moveLR = true;	// false = left, true = right
}

void pongGame(int pressed) {
	if (!drawnStuff) {
		// Draw white BG
		for (int i = 0; i < 256*192; i++) {
			VRAM_A[i] = 0x7FFF;
		}

		drawnStuff = true;
	}
	
	//if (arm9_errorColor) pong_color = 0x001B;

	rightpaddle_yPos = ball_yPos-16;
	if (rightpaddle_yPos <= 0) rightpaddle_yPos = 0;
	if (rightpaddle_yPos >= 159) rightpaddle_yPos = 159;

	// Draw ball (back)
	for (int y = ball_yPos-16; y <= ball_yPos+24; y++) {
		for (int k = ball_xPos-2; k <= ball_xPos+10; k++) {
			VRAM_A[y*256+k] = 0x7FFF;
		}
	}

	// Draw left paddle (back)
	for (int y = leftpaddle_yPos-4; y <= leftpaddle_yPos+36; y++) {
		for (int k = 8; k <= 16; k++) {
			VRAM_A[y*256+k] = 0x7FFF;
		}
	}

	// Draw right paddle (back)
	for (int y = rightpaddle_yPos-16; y <= rightpaddle_yPos+48; y++) {
		for (int k = 240; k <= 248; k++) {
			VRAM_A[y*256+k] = 0x7FFF;
		}
	}

	// Draw ball
	for (int y = ball_yPos; y <= ball_yPos+8; y++) {
		for (int k = ball_xPos; k <= ball_xPos+8; k++) {
			VRAM_A[y*256+k] = pong_color;
		}
	}

	// Draw left paddle
	for (int y = leftpaddle_yPos; y <= leftpaddle_yPos+32; y++) {
		for (int k = 8; k <= 16; k++) {
			VRAM_A[y*256+k] = pong_color;
		}
	}

	// Draw right paddle
	for (int y = rightpaddle_yPos; y <= rightpaddle_yPos+32; y++) {
		for (int k = 240; k <= 248; k++) {
			VRAM_A[y*256+k] = pong_color;
		}
	}

	if (ball_moveUD==false) {
		ball_yPos -= ball_ySpeed;
		if (ball_yPos <= 0) ball_moveUD = true;
	} else if (ball_moveUD==true) {
		ball_yPos += ball_ySpeed;
		if (ball_yPos >= 184) ball_moveUD = false;
	}

	if (ball_moveLR==false) {
		ball_xPos -= 2;
		if ((ball_yPos > leftpaddle_yPos-8) && (ball_yPos < leftpaddle_yPos+32)) {
			if (ball_xPos == 16) {
				ball_moveLR = true;
				if (ball_ySpeed != 6) {
					ball_ySpeed++;	// Increase Y speed of ball
				}
			}
		} else {
			if (ball_xPos <= 0) {
				// Restart minigame
				for (int i = 0; i < 256*192; i++) {
					VRAM_A[i] = 0x7FFF;
				}
				restartGame();
			}
		}
	} else if (ball_moveLR==true) {
		ball_xPos += 2;
		if (ball_xPos == 236) {
			ball_moveLR = false;
			if (ball_ySpeed != 6) {
				ball_ySpeed++;	// Increase Y speed of ball
			}
		}
	}

	scanKeys();
	int held = keysHeld();

	// Control left paddle
	if (held & KEY_UP) {
		leftpaddle_yPos -= 4;
		if (leftpaddle_yPos <= 0) leftpaddle_yPos = 0;
	}
	if (held & KEY_DOWN) {
		leftpaddle_yPos += 4;
		if (leftpaddle_yPos >= 159) leftpaddle_yPos = 159;
	}

	// Control right paddle
	/*if (held & KEY_X) {
		rightpaddle_yPos--;
		if (rightpaddle_yPos <= 0) rightpaddle_yPos = 0;
	}
	if (held & KEY_B) {
		rightpaddle_yPos++;
		if (rightpaddle_yPos >= 156) rightpaddle_yPos = 156;
	}*/

	if (pressed & KEY_SELECT) {
		screenMode++;
		restartGame();
		drawnStuff = false;
	}

	swiWaitForVBlank();
}