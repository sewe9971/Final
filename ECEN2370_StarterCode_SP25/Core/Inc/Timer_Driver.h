/*
 * Timer_Driver.h
 *
 *  Created on: Apr 30, 2025
 *      Author: nowth
 */

#ifndef INC_TIMER_DRIVER_H_
#define INC_TIMER_DRIVER_H_

#include "ApplicationCode.h"

void Timer_Init(void);
int Timer_Count(void);
void Timer_Start(void);
void TIM_Error_Handler(void);

#endif /* INC_TIMER_DRIVER_H_ */
