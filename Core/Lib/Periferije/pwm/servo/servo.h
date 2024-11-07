/*
 * servo.h
 *
 *  Created on: Nov 4, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_PWM_SERVO_SERVO_H_
#define LIB_PERIFERIJE_PWM_SERVO_SERVO_H_

void servo_init();
void servo_set_position(float percent);
void servo_set_pwm(float duty_cycle);

#endif /* LIB_PERIFERIJE_PWM_SERVO_SERVO_H_ */
