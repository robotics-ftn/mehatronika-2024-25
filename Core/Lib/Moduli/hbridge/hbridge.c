/*
 * hbridge.c
 *
 *  Created on: Nov 30, 2024
 *      Author: dexter
 */


#include "hbridge.h"
#include "stm32f401xe.h"

#define H_PSC (1 - 1)
#define H_ARR (4200 - 1)
#define V_2_DUC(x) (x * H_ARR/12.0) // voltage to duty cycle

typedef enum
{
	CW,
	CCW
}DIR_t;


/* direction */
static void m1_set_dir(DIR_t dir);
static void m2_set_dir(DIR_t dir);
/* duty cycle - PWM */
static void m1_set_pwm(uint32_t duc);
static void m2_set_pwm(uint32_t duc);


void
hbridge_init() {
	/*
	 * Init Motor1
	 *
	 * M1DIR: PA8 	Digital output
	 * M1FLT: PB10 	Digital input -> pull up
	 * M1PWM: PC7	TIM3 CH2 (AF2)
	 * M1SLP: PA10	Digital output -> output high!!!
	 */

	// init clock for GPIOA, GPIOB, GPIOC
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

	//PA8 M1DIR
	GPIOA->MODER &= ~(0b11 << 8 * 2);
	GPIOA->MODER |=  (0b01 << 8 * 2);

	//PB10 M1FLT
	GPIOB->MODER &= ~(0b11 << 10 * 2);
	GPIOB->PUPDR &= ~(0b11 << 10 * 2);
	GPIOB->PUPDR |=  (0b01 << 10 * 2); 	// pull-up

	//PA10 M1SLP
	GPIOA->MODER &= ~(0b11 << 10 * 2);
	GPIOA->MODER |=  (0b01 << 10 * 2);
	GPIOA->ODR 	 |=  (0b1  << 10); 		// enable driver

	//PC7 AF2
	GPIOC->MODER  &= ~(0b11 	<< 7 * 2);
	GPIOC->MODER  |=  (0b10 	<< 7 * 2);
	GPIOC->AFR[0] &= ~(0b1111 	<< 7 * 4);
	GPIOC->AFR[0] |=  (2 	  	<< 7 * 4); // AF2 (TIM3 CH2)

	// TIM3 CH2 init
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	/*
	 * 20kHz
	 */
	TIM3->PSC = H_PSC;
	TIM3->ARR = H_ARR;

	// PWM mode 1
	TIM3->CCMR1 &= ~(0b111 << 12);
	TIM3->CCMR1 |=  (0b110 << 12);

	TIM3->CCMR1 |= (0b1 << 11);

	TIM3->CR1 |= (0b1 << 7);

	TIM3->EGR |= (0b1 << 0);

	TIM3->CR1 &= ~(0b11 << 5);

	// ch2 enable
	TIM3->CCER |= (0b1 << 4);
	//enable timer
	TIM3->CR1 |= (0b1 << 0);

	/* ------------------------------ */
	/*
	 * Init motor 2:
	 *
	 * M2DIR: PA9 Digital output
	 * M2FLT: PA6 Digital input
	 * M2PWM: PB6 TIM4_CH1 (AF2)
	 * M2SLP: PB5 Digital output
	 */
	//PA9 M2DIR
	GPIOA->MODER &= ~(0b11 << 9 * 2);
	GPIOA->MODER |=  (0b01 << 9 * 2);

	//PA6 M2FLT
	GPIOA->MODER &= ~(0b11 << 6 * 2);
	GPIOA->PUPDR &= ~(0b11 << 6 * 2);
	GPIOA->PUPDR |=  (0b01 << 6 * 2); 	// pull-up

	//PB5 M2SLP
	GPIOB->MODER &= ~(0b11 << 5 * 2);
	GPIOB->MODER |=  (0b01 << 5 * 2);
	GPIOB->ODR 	 |=  (0b1  << 5); 		// enable driver


	//PB6 AF2
	GPIOB->MODER  &= ~(0b11 	<< 6 * 2);
	GPIOB->MODER  |=  (0b10 	<< 6 * 2);
	GPIOB->AFR[0] &= ~(0b1111 	<< 6 * 4);
	GPIOB->AFR[0] |=  (2 	  	<< 6 * 4); // AF2 (TIM4 CH1)

	// TIM3 CH2 init
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	/*
	 * 20kHz
	 */
	TIM4->PSC = H_PSC;
	TIM4->ARR = H_ARR;

	// PWM mode 1
	TIM4->CCMR1 &= ~(0b111 << 4);
	TIM4->CCMR1 |=  (0b110 << 4);

	// Output compare 1 preload enable
	TIM4->CCMR1 |= (0b1 << 3);

	TIM4->CR1 |= (0b1 << 7);

	TIM4->EGR |= (0b1 << 0);

	TIM4->CR1 &= ~(0b11 << 5);

	// ch1 enable
	TIM4->CCER |= (0b1 << 0);
	//enable timer
	TIM4->CR1 |= (0b1 << 0);

}

void
hbridge_voltage(float v_m1, float v_m2) {
	uint32_t duc1, duc2;

	// Motor1
	if (v_m1 < 0)
	{
		duc1 = V_2_DUC(-v_m1);
		m1_set_dir(CW);
	}
	else
	{
		duc1 = V_2_DUC(v_m1);
		m1_set_dir(CCW);
	}

	if (v_m2 < 0)
	{
		duc2 = V_2_DUC(-v_m2);
		m2_set_dir(CW);
	}
	else
	{
		duc2 = V_2_DUC(v_m2);
		m2_set_dir(CCW);
	}

	m1_set_pwm(duc1);
	m2_set_pwm(duc2);
}


/* direction */
static void
m1_set_dir(DIR_t dir) {
	switch(dir)
	{
	case CW:
		GPIOA->ODR &= ~(0b1 << 8);
		break;

	case CCW:
		GPIOA->ODR |=  (0b1 << 8);
		break;

	default:
		return;
	}
}

static void
m2_set_dir(DIR_t dir) {
	switch(dir)
	{
	case CW:
		GPIOA->ODR |= (0b1 << 9);
		break;
	case CCW:
		GPIOA->ODR &= ~(0b1 << 9);
		break;
	default:
		return;
	}
}

/* duty cycle - PWM */
static void
m1_set_pwm(uint32_t duc) {
	duc = duc > H_ARR ? H_ARR : duc;
	TIM3->CCR2 = duc;
}

static void
m2_set_pwm(uint32_t duc) {
	duc = duc > H_ARR ? H_ARR : duc;
	TIM4->CCR1 = duc;
}

