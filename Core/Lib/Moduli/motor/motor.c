/*
 * motor.c
 *
 *  Created on: Nov 30, 2024
 *      Author: dexter
 */
#include "motor.h"
#include "Moduli/hbridge/hbridge.h"
#include "malloc.h"

void
bdc_motors_init(Motor_t* motor, float Kp, float Kd, float Ki, float max_speed) {
	pid_init(&motor->pid, Kp, Ki, Kd, -12.0, 12.0);
	motor->max_speed = max_speed;
	motor->ref_speed = 0;
	motor->measured_speed = 0;

}

