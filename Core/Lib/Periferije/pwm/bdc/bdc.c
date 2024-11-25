/*
 * bdc.c
 *
 *  Created on: Nov 25, 2024
 *      Author: dexter
 */

#include "bdc.h"

static void motor1_set_dir(motor_dir_t dir);
static void motor2_set_dir(motor_dir_t dir);

static void motor1_set_pwm(int duc);
static void motor2_set_pwm(int duc);


void
motor1_init() {
	/*
	 * PA8
	 * TIM3 CH2
	 * 20 kHz
	 */

	RCC->AHB1ENR |= (
			RCC_AHB1ENR_GPIOAEN |
			RCC_AHB1ENR_GPIOBEN |
			RCC_AHB1ENR_GPIOCEN);

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	/* Init GPIOA PA8  Digital output*/

	// DIR pin
	GPIOA->MODER  &= ~(0b11 << 8 * 2);
	GPIOA->MODER  |=  (0b01 << 8 * 2);

	// FLT pin
	GPIOB->MODER &= ~(0b11 << 10 * 2);
	GPIOB->PUPDR &= ~(0b11 << 10 * 2);
	GPIOB->PUPDR |=  (0b01 << 10 * 2); 	// pull-up

	// SLP pin - output
	GPIOA->MODER &= ~(0b11 << 10 * 2);
	GPIOA->MODER |=  (0b01 << 10 * 2); 	// output
	GPIOA->ODR |= (0b1 << 10);

	/* Init GPIOC PC7 (AF2) TIM3 CH2*/
	GPIOC->MODER  &= ~(0b11 << 7 * 2);
	GPIOC->MODER  |=  (0b10 << 7 * 2);
	GPIOC->AFR[0] &= ~(0b1111 << 7 * 4);
	GPIOC->AFR[0] |=  (2 << 7 * 4);

	/* Init TIM3 */
	TIM3->PSC = MOTOR_PSC;
	TIM3->ARR = MOTOR_ARR;

	// rezim rada PWM Mode 1, logicka jedinica
	// sve dok je CNT < CCR
	TIM3->CCMR1 &= ~(0b111 << 12);
	TIM3->CCMR1 |=  (0b110 << 12);

	// enable preload
	TIM3->CCMR1 |= (0b1 << 11);

	TIM3->CR1 |= (0b1 << 7);

	//enable CH2 output
	TIM3->CCER |= (0b1 << 4);

	TIM3->EGR |= (0b1 << 0);
	// Enable counter
	TIM3->CR1 |= (0b1 << 0);

}
void
motor2_init() {
	//TODO: implement
}

void
motor1_set_dir(motor_dir_t dir) {
	switch(dir){
	case CW: //clockwise
		GPIOA->ODR &= ~(0b1 << 8);
		break;
	case CCW: // counter clockwise
		GPIOA->ODR |= (0b1 << 8);
		break;

	default: // stop

	}
}

void
motor2_set_dir(motor_dir_t dir) {
	//TODO: implement
}

void
motor1_set_pwm(int duc) {
	if (duc > 0)
	{
		motor1_set_dir(CW);
		TIM3->CCR2 = (uint32_t)duc;
	}
	else if (duc < 0)
	{
		motor1_set_dir(CCW);
		TIM3->CCR2 = (uint32_t)(-duc);
	}
	else
	{
		TIM3->CCR2 = 0;
	}

}

void
motor2_set_pwm(int duc) {
	//TODO: implement
}

void
motor1_set_voltage(float v) {
	/* izvrsiti interpolaciju naponu od +/- 12 volti do +/- ARR */
	int duc;
	// saturacija napona
	v = v < -12.0 ? -12.0 : (v > 12.0 ? 12 : v);

	//interpolacija napona
	duc = (int)((MOTOR_ARR / 12.0) * v);
	motor1_set_pwm(duc);
}

void
motor2_set_voltage(float v) {
	//TODO: implement
}
