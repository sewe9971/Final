/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

/* Static variables */


extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#endif // COMPILE_TOUCH_FUNCTIONS

int board[6][7] = {{0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0 ,0, 0, 0, 0},
							{0, 0, 0 ,0, 0, 0, 0},
							{0, 0, 0 ,0, 0, 0, 0},
							{0, 0, 0 ,0, 0, 0, 0},
							{0, 0, 0 ,0, 0, 0, 0}};

int position = 0;
int turn = 1;
int mode = 0;

void ApplicationInit(void)
{
	initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);

    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	RNG_Init();
	buttonInitInterrupt();

	#endif // COMPILE_TOUCH_FUNCTIONS
}

void LCD_Visual_Demo(void)
{
	visualDemo();
}

void LCD_Start_Screen(void)
{
	mode = 0;
	startScreen();
	LCD_Main_Menu_Polling();
}

void gameScreen1P(void) {

	mode = 1;

	LCD_Clear(0,LCD_COLOR_WHITE);

	turn = 1;

	drawBoard(board);

	position = 3;

	drawPosition(position, turn);

	HAL_Delay(400);

	polling1P();

}

void polling1P(void) {
	int letGo = 0;
	while (1) {

		if (checkWin() != 0) {
			break;
		}

		if (turn == 2) {
			HAL_Delay(100);
			position = RNG_Random() % 7;

			LCD_Clear(0,LCD_COLOR_WHITE); // TEMP
			drawBoard(board);

			while (place() != 1) {
				position = RNG_Random() % 7;
			}
			drawPosition(position, turn);

			HAL_Delay(800);

			LCD_Clear(0,LCD_COLOR_WHITE);
			drawBoard(board);
			HAL_Delay(300);
			position = 3;
			turn = 1;
			drawPosition(position, turn);

			if (checkWin() != 0) {
				break;
			}

			continue;
		}

		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			if ((StaticTouchData.y <= LCD_PIXEL_HEIGHT - 104) & (StaticTouchData.x <= LCD_PIXEL_WIDTH / 2)) { // Pressed on 1P Button
				if (letGo == 0) {
					letGo = 1;
					position = (position + 6) % 7;
					LCD_Clear(0,LCD_COLOR_WHITE); // TEMP
					drawBoard(board);
					drawPosition(position, turn);
					HAL_Delay(300);
				}
			}

			if ((StaticTouchData.y <= LCD_PIXEL_HEIGHT - 104) & (StaticTouchData.x >= LCD_PIXEL_WIDTH / 2)) { // Pressed on 1P Button
				if (letGo == 0) {
					letGo = 1;
					position = (position + 1) % 7;
					LCD_Clear(0,LCD_COLOR_WHITE); // TEMP
					drawBoard(board);
					drawPosition(position, turn);
					HAL_Delay(300);
				}
			}
			if (StaticTouchData.y > LCD_PIXEL_HEIGHT - 104) {
				if (place() == 1) {
					LCD_Clear(0,LCD_COLOR_WHITE);
					drawBoard(board);
					HAL_Delay(300);
					position = 3;
					if (turn == 1) {
						turn = 2;
					} else {
						turn = 1;
					}
					drawPosition(position, turn);
				}
				if (checkWin() != 0) {
					break;
				}
			}
		} else {
			/* Touch not pressed */
			letGo = 0;
			if (checkWin() != 0) {
				break;
			}
		}
	}

	if (checkWin() == 1) {
		// RED WINS!
		LCD_Clear(0,LCD_COLOR_RED);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_SetFont(&Font16x24);

		LCD_DisplayChar(60,80,'P');
		LCD_DisplayChar(70,80,'l');
		LCD_DisplayChar(77,80,'a');
		LCD_DisplayChar(88,80,'y');
		LCD_DisplayChar(100,80,'e');
		LCD_DisplayChar(110,80,'r');

		LCD_DisplayChar(127,80,'W');
		LCD_DisplayChar(140,80,'i');
		LCD_DisplayChar(153,80,'n');
		LCD_DisplayChar(165,80,'s');
	}

	if (checkWin() == 2) {
		// YELLOW WINS!
		LCD_Clear(0,LCD_COLOR_YELLOW);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_SetFont(&Font16x24);

		LCD_DisplayChar(70,80,'B');
		LCD_DisplayChar(83,80,'o');
		LCD_DisplayChar(95,80,'t');

		LCD_DisplayChar(117,80,'W');
		LCD_DisplayChar(130,80,'i');
		LCD_DisplayChar(143,80,'n');
		LCD_DisplayChar(155,80,'s');
	}

	if (checkWin() == 3) {
		// TIE!
		LCD_Clear(0,LCD_COLOR_BLACK);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetFont(&Font16x24);

		LCD_DisplayChar(70,80,'T');
		LCD_DisplayChar(83,80,'i');
		LCD_DisplayChar(95,80,'e');

		LCD_DisplayChar(117,80,'G');
		LCD_DisplayChar(132,80,'a');
		LCD_DisplayChar(145,80,'m');
		LCD_DisplayChar(160,80,'e');
	}

	for (int i=0; i<6; i++) {
		for (int j=0; j<7; j++) {
			board[i][j] = 0;
		}
	}

	HAL_Delay(1500);
	LCD_Start_Screen();
}

void gameScreen2P(void) {
	mode = 2;
	turn = 1;

	LCD_Clear(0,LCD_COLOR_WHITE); // TEMP

	drawBoard(board);

	position = 3;

	drawPosition(position, turn);

	HAL_Delay(400);

	polling2P();

}

int place(void) {
	int bottom = 6;
	for (int i=0; i < 6; i++) {
		if (board[i][position] != 0) {
			bottom = i;
			break;
		}
	}
	if (bottom == 0) {
		return(0);
	}
	board[bottom - 1][position] = turn;
	return(1);
}

int checkWin() {
	int win = 0;

	// Check Vertical
	for (int i=0; i < 3; i++) {
		for (int j=0; j < 7; j++) {
			if ((((board[i][j] == board[i + 1][j]) & (board[i+2][j] == board[i + 3][j])) & (board[i][j] == board[i + 2][j])) & (board[i][j] != 0)) {
				win = board[i][j];
			}
		}
	}
	// Check Horizontal
	for (int i=0; i < 6; i++) {
		for (int j=0; j < 4; j++) {
			if ((((board[i][j] == board[i][j + 1]) & (board[i][j + 2] == board[i][j + 3])) & (board[i][j] == board[i][j + 2])) & (board[i][j] != 0)) {
				win = board[i][j];
			}
		}
	}
	// Check NW Diagonal
	for (int i=0; i < 3; i++) {
		for (int j=0; j < 4; j++) {
			if ((((board[i][j] == board[i + 1][j + 1]) & (board[i + 2][j + 2] == board[i + 3][j + 3])) & (board[i][j] == board[i + 2][j + 2])) & (board[i][j] != 0)) {
				win = board[i][j];
			}
		}
	}
	// Check NW Diagonal
	for (int i=3; i < 6; i++) {
		for (int j=0; j < 4; j++) {
			if ((((board[i][j] == board[i - 1][j + 1]) & (board[i - 2][j + 2] == board[i - 3][j + 3])) & (board[i][j] == board[i - 2][j + 2])) & (board[i][j] != 0)) {
				win = board[i][j];
			}
		}
	}

	// Check for Tie
	int tie = 1;
	for (int i=0; i<7; i++) {
		if (board[0][i] == 0) {
			tie = 0;
		}
	}
	if (tie == 1) {
		win = 3;
	}

	return(win);
}

void polling2P(void) {
	int letGo = 0;
	while (1) {
		if (checkWin() != 0) {
			break;
		}
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			if ((StaticTouchData.y <= LCD_PIXEL_HEIGHT - 104) & (StaticTouchData.x <= LCD_PIXEL_WIDTH / 2)) { // Pressed on 1P Button
				if (letGo == 0) {
					letGo = 1;
					position = (position + 6) % 7;
					LCD_Clear(0,LCD_COLOR_WHITE); // TEMP
					drawBoard(board);
					drawPosition(position, turn);
					HAL_Delay(300);
				}
			}

			if ((StaticTouchData.y <= LCD_PIXEL_HEIGHT - 104) & (StaticTouchData.x >= LCD_PIXEL_WIDTH / 2)) { // Pressed on 1P Button
				if (letGo == 0) {
					letGo = 1;
					position = (position + 1) % 7;
					LCD_Clear(0,LCD_COLOR_WHITE); // TEMP
					drawBoard(board);
					drawPosition(position, turn);
					HAL_Delay(300);
				}
			}
			if (StaticTouchData.y > LCD_PIXEL_HEIGHT - 104) {
				if (place() == 1) {
					LCD_Clear(0,LCD_COLOR_WHITE);
					drawBoard(board);
					HAL_Delay(300);
					position = 3;
					if (turn == 1) {
						turn = 2;
					} else {
						turn = 1;
					}
					drawPosition(position, turn);
				}
				if (checkWin() != 0) {
					break;
				}
			}
		} else {
			/* Touch not pressed */
			letGo = 0;
			if (checkWin() != 0) {
				break;
			}
		}
	}

	if (checkWin() == 1) {
		// RED WINS!
		LCD_Clear(0,LCD_COLOR_RED);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_SetFont(&Font16x24);

		LCD_DisplayChar(70,80,'R');
		LCD_DisplayChar(83,80,'e');
		LCD_DisplayChar(95,80,'d');

		LCD_DisplayChar(117,80,'W');
		LCD_DisplayChar(130,80,'i');
		LCD_DisplayChar(143,80,'n');
		LCD_DisplayChar(155,80,'s');

	}

	if (checkWin() == 2) {
		// YELLOW WINS!
		LCD_Clear(0,LCD_COLOR_YELLOW);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_SetFont(&Font16x24);

		LCD_DisplayChar(60,80,'Y');
		LCD_DisplayChar(72,80,'e');
		LCD_DisplayChar(80,80,'l');
		LCD_DisplayChar(85,80,'l');
		LCD_DisplayChar(92,80,'o');
		LCD_DisplayChar(106,80,'w');

		LCD_DisplayChar(127,80,'W');
		LCD_DisplayChar(140,80,'i');
		LCD_DisplayChar(153,80,'n');
		LCD_DisplayChar(165,80,'s');

	}

	if (checkWin() == 3) {
		// TIE!
		LCD_Clear(0,LCD_COLOR_BLACK);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetFont(&Font16x24);

		LCD_DisplayChar(70,80,'T');
		LCD_DisplayChar(83,80,'i');
		LCD_DisplayChar(95,80,'e');

		LCD_DisplayChar(117,80,'G');
		LCD_DisplayChar(132,80,'a');
		LCD_DisplayChar(145,80,'m');
		LCD_DisplayChar(160,80,'e');
	}

	for (int i=0; i<6; i++) {
		for (int j=0; j<7; j++) {
			board[i][j] = 0;
		}
	}

	HAL_Delay(1500);

	LCD_Start_Screen();
}

void LCD_Main_Menu_Polling(void) {
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {

			if (((StaticTouchData.y <= LCD_PIXEL_HEIGHT - 130) & (StaticTouchData.y >= LCD_PIXEL_HEIGHT - 290)) & (StaticTouchData.x <= 100)) { // Pressed on 1P Button
				gameScreen1P();
				// break;
			}

			if (((StaticTouchData.y <= LCD_PIXEL_HEIGHT - 130) & (StaticTouchData.y >= LCD_PIXEL_HEIGHT - 290)) & (StaticTouchData.x >= LCD_PIXEL_WIDTH - 100)) { // Pressed on 1P Button
				gameScreen2P();
				// break;
			}

		}
	}
}

#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);

		} else {
			/* Touch not pressed */
			printf("Not Pressed\n\n");
			LCD_Clear(0, LCD_COLOR_GREEN);

		}
	}
}

#endif // COMPILE_TOUCH_FUNCTIONS

void buttonInitInterrupt(void) {
	Button_Init_Interrupt();
}

void EXTI0_IRQHandler() {
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);

	if (!(mode == 1 && turn == 2)) {
		if (place() == 1) {
			LCD_Clear(0,LCD_COLOR_WHITE);
			drawBoard(board);
			position = 3;
			if (turn == 1) {
				turn = 2;
			} else {
				turn = 1;
			}
			drawPosition(position, turn);
		}
	}

	HAL_EXTI_ClearPending(EXTI_GPIOA, EXTI_TRIGGER_RISING);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
