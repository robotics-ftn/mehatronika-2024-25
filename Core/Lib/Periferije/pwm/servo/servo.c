/*
 * servo.c
 *
 *  Created on: Nov 4, 2024
 *      Author: dexter
 */
#include "servo.h"
#include "stm32f401xe.h"

void
servo_init() {
	/*
	 * TIM10 CH1
	 * f_pwm = 50Hz
	 * PB8 AF3
	 */

	// Enable GPIO clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	// Enable TIM clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	GPIOB->MODER &= ~(0b11 << 8 * 2);
	GPIOB->MODER |=  (0b10 << 8 * 2);

	GPIOB->AFR[1] &= ~(0b1111 << 0 * 4);
	GPIOB->AFR[1] |=  (3      << 0 * 4);

	// Init timer

	//50Hz
	TIM10->PSC = 84 -1; 	// --> 1 MHz
	TIM10->ARR = 20000 - 1;	// 50 Hz

	// rezim rada PWM Mode 1, logicka jedinica
	// sve dok je CNT < CCR
	TIM10->CCMR1 &= ~(0b111 << 4);
	TIM10->CCMR1 |=  (0b110 << 4);

	// enable preload
	TIM10->CCMR1 |= (0b1 << 3);

	TIM10->CR1 |= (0b1 << 7);

	//enable CH1 output
	TIM10->CCER |= (0b1 << 0);


	TIM10->EGR |= (0b1 << 0);
	// Enable counter
	TIM10->CR1 |= (0b1 << 0);

}

void
servo_set_position(float percent) {
	float duty_cycle;

	duty_cycle = (1.0 + percent) / 20.0;

	servo_set_pwm(duty_cycle);
}
/*
 * duty_cyle is in percents [0, 1]
 */
void
servo_set_pwm(float duty_cycle) {
	uint16_t duty_cyle_arr;
	duty_cyle_arr = duty_cycle * (20000 - 1);

	TIM10->CCR1 = duty_cyle_arr;
}


