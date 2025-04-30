/*
 * RNG_Driver.c
 *
 *  Created on: Apr 27, 2025
 *      Author: nowth
 */

#include "RNG_Driver.h"

RNG_HandleTypeDef hrng1;

void RNG_Init(void) {

	hrng1.Instance = RNG;

	if (HAL_RNG_Init(&hrng1) != HAL_OK) {
		RNG_Error_Handler();
	}
}

void RNG_Error_Handler(void) {
	while(1) {
		// Do nothing
	}
}

uint32_t RNG_Random(void) {
	uint32_t num = 0;
	uint32_t status = HAL_RNG_GenerateRandomNumber(&hrng1, &num);
	if (status != HAL_OK) {
		RNG_Error_Handler();
	}
	return(num);
}
