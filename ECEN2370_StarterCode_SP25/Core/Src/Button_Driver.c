/*
 * Button_Driver.c
 *
 *  Created on: Feb 11, 2025
 *      Author: nowth
 */

#include "Button_Driver.h"

bool IsButtonPressed() {
	return(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0));
}

void Button_Init_Interrupt() {
	GPIO_InitTypeDef PCFG = {0};
	PCFG.Pin = GPIO_PIN_0;
	PCFG.Pull = GPIO_NOPULL;
	PCFG.Mode = GPIO_MODE_IT_RISING;
	PCFG.Alternate = RESET;
	PCFG.Speed = GPIO_SPEED_FREQ_LOW;
	// PCFG.PinInterruptMode = FALLING_RISING_IRQ; // Gone I guess

	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &PCFG);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
