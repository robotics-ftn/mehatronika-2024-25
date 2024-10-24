/*
 * gpio.c
 *
 *  Created on: Oct 17, 2024
 *      Author: dexter
 */

#include "gpio.h"

void GPIO_LED_Init()
{
	// Enable GPIOA clock
	RCC->AHB1ENR |= (0b1 << 0);

	// LED -> PA5 (digitalni izlaz)
	GPIOA->MODER &= ~(0b1 << 11); // clear bit 11
	GPIOA->MODER |=  (0b1 << 10); // set bit 10

	// Push-pull
	GPIOA->OTYPER &= ~(0b1 << 5);
}

void GPIO_LED_Toggle()
{
	GPIOA->ODR ^= (0b1 << 5);
}


