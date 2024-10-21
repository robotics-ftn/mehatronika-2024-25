/*
 * encoder.c
 *
 *  Created on: Oct 21, 2024
 *      Author: dexter
 */

#include "encoder.h"
#include "stm32f401xe.h"

static uint32_t prev_cnt;

static void encoder1_gpio_init();
static void encoder1_tim_init();
static void counter_init();

void
encoder1_init() {
	encoder1_gpio_init();
	encoder1_tim_init();

	counter_init();

}

int32_t
encoder1_inc_delta() {
	uint32_t cnt = TIM2->CNT;
	int32_t delta = (int32_t)(cnt - prev_cnt);
	prev_cnt = cnt;

	return delta;
}

/* ----- PRIVATE ----- */
static void
encoder1_gpio_init() {

	// PA15 --> TIM2 CH1 (Alt. fun. 2)
	// PB3  --> TIM2 CH2 (Alt. fun. 2)

	// Enable clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// Podesimo mod: Alternativni rezim

	//PA15 alt fun 2
	GPIOA->MODER &= ~(0b11 << 15 * 2);
    GPIOA->MODER |=  (0b10 << 15 * 2);

    GPIOA->AFR[1] &= ~(0b1111 << 28);
    GPIOA->AFR[1] |=  (0b0001 << 28);

    // PB3 alt fun 2
	GPIOB->MODER &= ~(0b11 << 3 * 2);
    GPIOB->MODER |=  (0b10 << 3 * 2);

    GPIOB->AFR[0] &= ~(0b1111 << 3 * 4);
    GPIOB->AFR[0] |=  (0b0001 << 3 * 4);

}

static void
encoder1_tim_init() {

	// enable clock for TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	//Encoder mode 3 - kvadraturno brojanje
	TIM2->SMCR &= ~(0b111 << 0);
	TIM2->SMCR |=  (0b011 << 0);

	TIM2->ARR = 0xFFFFFFFF;
	TIM2->CNT = (0xFFFFFFFF >> 1);

	// update generation:
	// Re-initialize the counter and generates an update of the registers
	TIM2->EGR |= (0b1 << 0);
	// Enable Timer
	TIM2->CR1 |= (0b1 << 0);
}

static void
counter_init() {
	prev_cnt = (0xFFFFFFFF >> 1);
}

