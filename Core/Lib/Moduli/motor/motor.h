/*
 * motor.h
 *
 *  Created on: Nov 30, 2024
 *      Author: dexter
 */

#ifndef LIB_MODULI_MOTOR_MOTOR_H_
#define LIB_MODULI_MOTOR_MOTOR_H_

#include "Moduli/pid/pid.h"

typedef struct
{
	double max_speed;
	double ref_speed;
	double measured_speed;
	PID_t pid;
}Motor_t;

void bdc_motors_init(Motor_t* motor, float Kp, float Kd, float Ki, float max_speed);

#endif /* LIB_MODULI_MOTOR_MOTOR_H_ */
