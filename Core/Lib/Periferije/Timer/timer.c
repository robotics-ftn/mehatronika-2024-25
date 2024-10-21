/*
 * timer.c
 *
 *  Created on: Oct 14, 2024
 *      Author: dexter
 */

#include "timer.h"

volatile uint32_t system_ms = 0;
volatile sTimerFLags_t timer_flags;

void TIM1_Init()
{
	// RCC clock signal enable za TIM1
	RCC->APB2ENR |= (0b1 << 0);

	// Zeljena frekvencija: 1kHz
	// Koju imamo: 84 MHz

	TIM1->PSC = 84 - 1; 	// -> 1 MHz
	TIM1->ARR = 1000 - 1;	// -> 1 kHz

	// smer brojanja na gore
	TIM1->CR1 &= ~(0b1 << 4);
	// dozvola za prekid samo na overflow/underflow
	TIM1->CR1 |= (0b1 << 2);

	TIM1->EGR |= (0b1 << 0);

	//enable update interrupt
	TIM1->DIER |= (0b1 << 0);
	// upalimo tajmerski brojac (enable)
	TIM1->CR1 |= (0b1 << 0);

	NVIC->ISER[0] |= (0b1 << 25);

}

void TIM1_UP_TIM10_IRQHandler()
{
	// provera da li je doslo do update eventa
	if((TIM1->SR & (0b1 << 0)) == (0b1 << 0))
	{
		system_ms++;

		if(system_ms == (1000 * 100))
			timer_flags.flg_100s = 1;

		if(system_ms == (1000 * 85))
			timer_flags.flg_85s = 1;

		TIM1->SR &= ~(0b1 << 0); // ovo zahteva MCU od nas
	}
}
