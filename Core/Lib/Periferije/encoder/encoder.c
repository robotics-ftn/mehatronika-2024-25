/*
 * encoder.c
 *
 *  Created on: Oct 24, 2024
 *      Author: dexter
 */

#include "stm32f401xe.h"
#include "encoder.h"

#define ENC_MAX_CNT		0xFFFF
#define ENC_START_CNT	(ENC_MAX_CNT >> 1)

void
encoder_init_all() {
	encoder_desni_init();
	encoder_levi_init();
}

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
	TIM2->ARR = ENC_MAX_CNT - 1;
	TIM2->PSC = 0;

	TIM2->CCMR1 &= ~(0b11 << 0 | 0b11 << 8);
	TIM2->CCMR1 |=  (0b01 << 0 | 0b01 << 8); //IC1 -> TI1 & IC2 -> TI2

	// enable timer
	TIM2->CR1 |= (0b1 << 0);
	// init. counter (update generation)
	TIM2->EGR |= (0b1 << 0);
	TIM2->CNT = ENC_START_CNT - 1;

}


void
encoder_levi_init() {
	/*
	 * PA0  --> TIM5 CH1
	 * PA1  --> TIM5 CH2
	 */

	// Enable clock signal
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER &= ~(0b11 << 0 * 2);
	GPIOA->MODER |=  (0b10 << 0 * 2); // alt. fun.
	GPIOA->AFR[0] &= ~(0b1111 << 0 * 4);
	GPIOA->AFR[0] |=  (2 << 0 * 4);

	GPIOA->MODER &= ~(0b11 << 1 * 2);
	GPIOA->MODER |=  (0b10 << 1 * 2); // alt. fun.
	GPIOA->AFR[0] &= ~(0b1111 << 1 * 4);
	GPIOA->AFR[0] |=  (2 << 1 * 4);


	// Encoder 3 mode (count on both edges) - kvadraturno dekodiranje
	TIM5->SMCR &= ~(0b111 << 0);
	TIM5->SMCR |=  (0b011 << 0);

	// max inkremenata do koliko broji
	TIM5->ARR = ENC_MAX_CNT - 1;
	TIM5->PSC = 0;

	TIM5->CCMR1 &= ~(0b11 << 0 | 0b11 << 8);
	TIM5->CCMR1 |=  (0b01 << 0 | 0b01 << 8); //IC1 -> TI1 & IC2 -> TI2

	// enable timer
	TIM5->CR1 |= (0b1 << 0);
	// init. counter (update generation)
	TIM5->EGR |= (0b1 << 0);

	TIM5->CNT = ENC_START_CNT - 1;



}

int32_t
encoder_desni_get_inc() {
	int32_t tmp = (TIM2->CNT - ENC_START_CNT);
	TIM2->CNT = ENC_START_CNT;
	return tmp;
}

int32_t
encoder_levi_get_inc() {
	int32_t tmp = (TIM5->CNT - ENC_START_CNT);
	TIM5->CNT = ENC_START_CNT;
	return tmp;
}
