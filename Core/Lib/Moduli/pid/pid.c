/*
 * pid.c
 *
 *  Created on: Nov 30, 2024
 *      Author: dexter
 */


#include "pid.h"
#include "memory.h"

void
pid_init(PID_t* pid, float Kp, float Ki, float Kd, float out_min, float out_max) {
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;

	pid->output = 0;
	pid->error[0] = 0;
	pid->error[1] = 0;

	pid->out_max = out_max;
	pid->out_min = out_min;

}

void
pid_compute(PID_t* pid, float reference, float measured) {
	/*
	 * error = e[kT]
	 * error[0] = e[kT-T]
	 * error[1] = e[kT-2T]
	 */
	float error = reference - measured;
	pid->output += (
			pid->Kp * (error - pid->error[0]) +
			pid->Ki * error +
			pid->Kd * (error - 2 * pid->error[0] + pid->error[1])
	);

	pid->error[1] = pid->error[0];
	pid->error[0] = error;

	// saturacija
	if (pid->output > pid->out_max)
		pid->output = pid->out_max;
	if (pid->output < pid->out_min)
		pid->output = pid->out_min;

}
