/*
 * RNG_Driver.h
 *
 *  Created on: Apr 27, 2025
 *      Author: nowth
 */

#ifndef INC_RNG_DRIVER_H_
#define INC_RNG_DRIVER_H_

#include "ApplicationCode.h"

void RNG_Init(void);
void RNG_Error_Handler(void);
uint32_t RNG_Random(void);

#endif /* INC_RNG_DRIVER_H_ */
