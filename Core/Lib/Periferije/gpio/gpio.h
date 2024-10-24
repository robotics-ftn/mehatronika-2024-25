/*
 * gpio.h
 *
 *  Created on: Oct 17, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_GPIO_GPIO_H_
#define LIB_PERIFERIJE_GPIO_GPIO_H_

#include "stm32f4xx.h"

void GPIO_LED_Init();
void GPIO_LED_Toggle();

void GPIO_Btn_Init();
uint8_t GPIO_Btn_Read();

#endif /* LIB_PERIFERIJE_GPIO_GPIO_H_ */
