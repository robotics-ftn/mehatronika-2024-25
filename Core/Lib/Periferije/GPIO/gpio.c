/*
 * gpio.c
 *
 *  Created on: Oct 14, 2024
 *      Author: dexter
 */
#include "gpio.h"

void LED_Init()
{
	// Ukljucivanje clock signala na AHB1
	RCC->AHB1ENR |= (0b1 << 0);

	//Postavljamo Pin 5 na Portu A (PA5) kao izlazni

	// prvo upisemo 0b00 na bitove 10 i 11
	GPIOA->MODER &= ~(0b11 << 10);
	// upisemo 0b01 na bitove 10 i 11
	GPIOA->MODER |= (0b01 << 10);

	//podesimo kao push-pull
	GPIOA->OTYPER &= ~(0b1 << 5);
}

void LED_Toggle()
{
	// za pocetak je samo palimo
	GPIOA->ODR ^= (0b1 << 5);
}

void Button_Init()
{

}

uint8_t Button_Read()
{

}
