/*
 * ApplicationCode.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Xavion
 */

#include "LCD_Driver.h"
#include "stm32f4xx_hal.h"

#include <stdio.h>
#include "RNG_Driver.h"
#include "Button_Driver.h"


#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_

void ApplicationInit(void);
void LCD_Visual_Demo(void);
void LCD_Start_Screen(void);

void LCD_Main_Menu_Polling(void);
void polling1P(void);
void polling2P(void);

int place(void);
int checkWin();

void buttonInitInterrupt(void);

#if (COMPILE_TOUCH_FUNCTIONS == 1)
void LCD_Touch_Polling_Demo(void);
#endif // (COMPILE_TOUCH_FUNCTIONS == 1)

#endif /* INC_APPLICATIONCODE_H_ */
