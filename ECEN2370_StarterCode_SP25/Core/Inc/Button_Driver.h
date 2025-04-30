/*
 * Button_Driver.h
 *
 *  Created on: Feb 11, 2025
 *      Author: nowth
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include <stdbool.h>
#include "STM32F4xx_hal.h"

#define BUTTON_PORT 0
#define BUTTON_PIN 0

#define BUTTON_PRESSED 0
#define BUTTON_UNPRESSED 1

bool IsButtonPressed();

void Button_Init_Interrupt();

#endif /* BUTTON_DRIVER_H_ */
