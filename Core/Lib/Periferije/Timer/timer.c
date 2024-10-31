/*
 * timer.c
 *
 *  Created on: Oct 17, 2024
 *      Author: dexter
 */

#include "timer.h"
#include "Moduli/odom/odom.h"

static const uint8_t odom_update_loop_ms = 5;

static uint32_t system_ms;
static uint32_t timeout_cnt;

timer_flgs_t timer_flgs = {
		.flg_100s   = 0,
		.flg_85s	= 0,
		.flg_timeout_start = 0,
		.flg_timeout_end = 0
};


static void
init_variables();

void TIM1_Init()
{
	// TIM1 je na APB2

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;


	// Podesavanje preskalera i ARR registra (registar za poredjenje sa brojacem)

	// 84 MHz -> 1 kHz

	/*
	 * 1. 84 MHz -> 1MHz ---- > ulaz u brojac
	 * 2. 1MHz -> 1kHz	 ---- > f-ja izlaza iz komparatora
	 */

	TIM1->PSC = 84 - 1; 	// --> 1MHz
	TIM1->ARR = 1000 - 1;

	//upcounter
	TIM1->CR1 &= ~(0b1 << 4);

	// ivicno poravnat nacin brojanja
	TIM1->CR1 &= ~(0b11 << 5);
	// prekid samo na overflow/underflow
	TIM1->CR1 |= (0b1 << 2);

	//dozvoli da tajmer generise prekid
	TIM1->DIER |= (0b1 << 0);

	// reinit. brojac
	TIM1->EGR |= (0b1 << 0);

	// NVIC enable
	NVIC->ISER[0] |= (0b1 << 25);

	init_variables();

	TIM1_Enable();

}

void TIM1_Enable()
{
	TIM1->CR1 |= (0b1 << 0);
}

void TIM1_Disable()
{
	TIM1->CR1 &= ~(0b1 << 0);
}

void
TIM1_timeout(uint32_t ms) {

	if (timer_flgs.flg_timeout_start)
		return;

	timer_flgs.flg_timeout_start = 1;
	timer_flgs.flg_timeout_end = 0;
	timeout_cnt = ms;
}


void TIM1_UP_TIM10_IRQHandler()
{
	if ((TIM1->SR & 0b1) == 0b1)
	{
		system_ms++;

		if ((system_ms % 1000 * 100) == 0)
			timer_flgs.flg_100s = 1;

		if ((system_ms % 1000 * 85) == 0)
			timer_flgs.flg_85s = 1;


		if (timer_flgs.flg_timeout_start) {
			timeout_cnt--;

			if (timeout_cnt == 0) {
				timer_flgs.flg_timeout_start = 0;
				timer_flgs.flg_timeout_end = 1;
			}
		}

		if (system_ms % odom_update_loop_ms == 0)
			odom_update(odom_update_loop_ms);

		TIM1->SR &= ~(0b1 << 0);
	}
}

static void
init_variables()
{
	system_ms = 0;
	timeout_cnt = 0;
}

