/*
 * bdc.h
 *
 *  Created on: Nov 25, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_PWM_BDC_BDC_H_
#define LIB_PERIFERIJE_PWM_BDC_BDC_H_

#include "stm32f401xe.h"

/*
 * M1DIR --> PA8 GPIO (DO)
 * M1PWM --> PC7 (TIM3_CH2)
 *
 * M2DIR --> PA9 GPIO (DO)
 * M2PWM --> PB6 (TIM4_CH1)
 */

#define MOTOR_PSC (2 - 1)
#define MOTOR_ARR (2100 - 1)

typedef enum
{
	CW,
	CCW,
	STOP
}motor_dir_t;


void motor1_init();
void motor2_init();

//void motor1_set_dir(motor_dir_t dir);
//void motor2_set_dir(motor_dir_t dir);
//
//void motor1_set_pwm(int duc);
//void motor2_set_pwm(int duc);

void motor1_set_voltage(float v);
void motor2_set_voltage(float v);

#endif /* LIB_PERIFERIJE_PWM_BDC_BDC_H_ */
