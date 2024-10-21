/*
 * gpio.h
 *
 *  Created on: Oct 14, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_GPIO_GPIO_H_
#define LIB_PERIFERIJE_GPIO_GPIO_H_

#include "stm32f401xe.h"

void LED_Init();
void LED_Toggle();

void Button_Init();
uint8_t Button_Read();

#endif /* LIB_PERIFERIJE_GPIO_GPIO_H_ */
