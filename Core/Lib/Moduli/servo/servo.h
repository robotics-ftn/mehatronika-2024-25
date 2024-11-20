/*
 * servo.h
 *
 *  Created on: Nov 14, 2024
 *      Author: dexter
 */

#ifndef LIB_MODULI_SERVO_SERVO_H_
#define LIB_MODULI_SERVO_SERVO_H_

#include <stdint.h>

void servo_set_position(float range);
void servo_set_pwm(uint16_t pwm);

#endif /* LIB_MODULI_SERVO_SERVO_H_ */
