/*
 * Timer_Driver.c
 *
 *  Created on: Apr 30, 2025
 *      Author: nowth
 */

#include "Timer_Driver.h"

extern TIM_HandleTypeDef htim2;

void Timer_Init(void) {
	__HAL_RCC_TIM2_CLK_ENABLE();

	htim2.Instance = TIM2;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Prescaler = 8399;
	htim2.Init.Period = 9999; // 1 Hz
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		TIM_Error_Handler();
	}

	HAL_TIM_Base_Start_IT(&htim2);
}

void TIM_Error_Handler(void) {
	while(1) {
	// Do nothing
	}
}

int Timer_Count(void) {
	return((int) __HAL_TIM_GET_COUNTER(&htim2));
}

void Timer_Start(void) {
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	htim2.Instance->CNT = 0;
	htim2.Instance->CR1 = (1 << 0);
}
