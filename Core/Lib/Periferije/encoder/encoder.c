/*
 * encoder.c
 *
 *  Created on: Oct 24, 2024
 *      Author: dexter
 */

#include "stm32f401xe.h"
#include "encoder.h"


void
encoder_desni_init() {
	// Enable clock signal
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// GPIO init

	/*
	 * PA5 ---> TIM2 CH1
	 * PB3 ---> TIM2 CH2
	 */

	// PA5 init
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER &= ~(0b11 << 5 * 2);
	GPIOA->MODER |=  (0b10 << 5 * 2); // alt. fun.
	GPIOA->AFR[0] &= ~(0b1111 << 5 * 4);
	GPIOA->AFR[0] |=  (0b0001 << 5 * 4);

	// PB3 init
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	GPIOB->MODER &= ~(0b11 << 3 * 2);
	GPIOB->MODER |=  (0b10 << 3 * 2); // alt. fun.
	GPIOB->AFR[0] &= ~(0b1111 << 3 * 4);
	GPIOB->AFR[0] |=  (0b0001 << 3 * 4);


	// Encoder 3 mode (count on both edges) - kvadraturno dekodiranje
	TIM2->SMCR &= ~(0b111 << 0);
	TIM2->SMCR |=  (0b011 << 0);

	// max inkremenata do koliko broji
	TIM2->ARR = 0xFFFF;

	// init. counter (update generation)
	TIM2->EGR |= (0b1 << 0);

	// enable timer
	TIM2->CR1 |= (0b1 << 0);

}

int32_t
encoder_desni_get_inc() {
	int32_t tmp = (int32_t)TIM2->CNT;
	TIM2->CNT = 0;
	return tmp;
}