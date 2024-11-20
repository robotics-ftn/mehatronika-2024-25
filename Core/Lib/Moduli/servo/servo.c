/*
 * servo.c
 *
 *  Created on: Nov 14, 2024
 *      Author: dexter
 */

#include "servo.h"
#include "stm32f401xe.h"
#define SERVO_ARR 19999

void
servo_set_position(float range) {
/*
 * range mora biti u rasponu od [0, 1]
 */
	servo_set_pwm((uint16_t)((1 + range) * SERVO_ARR/ 20.0));
}


void
servo_set_pwm(uint16_t pwm) {
	pwm = pwm > SERVO_ARR ? SERVO_ARR : (pwm < 0 ? 0 : pwm); // saturacija
	TIM10->CCR1 = pwm;
}
