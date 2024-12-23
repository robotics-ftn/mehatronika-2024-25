/*
 * pid.h
 *
 *  Created on: Nov 30, 2024
 *      Author: dexter
 */

#ifndef LIB_MODULI_PID_PID_H_
#define LIB_MODULI_PID_PID_H_

typedef struct
{
	double Kp;
	double Ki;
	double Kd;

	double error[2];

	double out_max;
	double out_min;

	double output;
}PID_t;


void pid_init(PID_t* pid, float Kp, float Ki, float Kd, float out_min, float out_max);
void pid_compute(PID_t* pid, float reference, float measured);

#endif /* LIB_MODULI_PID_PID_H_ */
