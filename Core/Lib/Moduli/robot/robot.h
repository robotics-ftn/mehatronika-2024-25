/*
 * robot.h
 *
 *  Created on: Dec 5, 2024
 *      Author: dexter
 */

#ifndef LIB_MODULI_ROBOT_ROBOT_H_
#define LIB_MODULI_ROBOT_ROBOT_H_

#include "Moduli/hbridge/hbridge.h"
#include "Moduli/motor/motor.h"
#include "Moduli/odom/odom.h"
#include "Periferije/encoder/encoder.h"

typedef struct
{
	double Kp_pos;
	double Kp_rot;

	double x_ref;
	double y_ref;
	double theta_ref;

	odom_t odom;
	Motor_t mot1;
	Motor_t mot2;

	unsigned char phase;

}robot_t;


void robot_init();

void robot_set_ref(float x, float y, float theta);
void odom_update_loop(uint32_t dt_ms);
void pos_update_loop();
void vel_update_loop();

#endif /* LIB_MODULI_ROBOT_ROBOT_H_ */
