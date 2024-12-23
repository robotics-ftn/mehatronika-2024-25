/*
 * robot.c
 *
 *  Created on: Dec 5, 2024
 *      Author: dexter
 */

#include "robot.h"
#include <math.h>
#include <stdlib.h>

static const float
L = 0.259, 				// razmak izmedju tockova enkodera u [m]
D = 0.078,
r = 0.078/2.0, 			// poluprecnik enkoderskih tockova
inc2rad = 0.00076699; 	// pretvaranje inkremenata enkodera u radijane

static robot_t robot = {0};

static float normalize_angle(float angle);

void
robot_set_ref(float x, float y, float theta) {
	if (robot.phase == 0)
	{
		robot.x_ref = x;
		robot.y_ref = y;
		robot.theta_ref = theta;
		robot.phase = 1;
	}
}

void
odom_update_loop(uint32_t dt_ms) {

	int32_t v_l_inc;
	int32_t v_d_inc;
	float v_l, v_d, dt_s;

	v_d_inc = encoder_desni_get_inc();
	v_l_inc = encoder_levi_get_inc();

	dt_s = dt_ms / 1000.0;

	v_l = v_l_inc * inc2rad * r / dt_s;
	v_d = v_d_inc * inc2rad * r / dt_s;

	robot.mot1.measured_speed = v_d;
	robot.mot2.measured_speed = v_l;

	robot.odom.v = (v_d + v_l) / 2.0;
	robot.odom.w = (v_d - v_l) / L;

	robot.odom.x += robot.odom.v * cos(robot.odom.theta + robot.odom.w * dt_s / 2.0) * dt_s;
	robot.odom.y += robot.odom.v * sin(robot.odom.theta + robot.odom.w * dt_s / 2.0) * dt_s;

	robot.odom.theta += robot.odom.w * dt_s;

	robot.odom.theta = normalize_angle(robot.odom.theta);
}

void
pos_update_loop() {
	float v;
	float w;

	float phi; //ugao  izmedju X ose i vektora ka targetu
	float dist;
	float phi_prim;

	float rot_error;
	float dist_error;

	phi = atan2(robot.y_ref - robot.odom.y, robot.x_ref - robot.odom.x);
	rot_error = (phi - robot.odom.theta) * robot.Kp_rot;

	switch(robot.phase)
	{
	case 1: // Rotacija ka targetu
		w = rot_error * robot.Kp_rot; //hbridge_voltage(rot_error, - rot_error);
		v = 0;
		break;

	case 2: // Translacija do targeta
		// Voditi racuna i o orijentaciji
		break;

	case 3: // Finalna orijentacija

		break;
	}

	robot.mot1.ref_speed = v + w * L / 2.0;
	robot.mot2.ref_speed = v - w * L / 2.0;
}

void
vel_update_loop() {
	// linearno ubrzanje

	// PID brzine levog i desnog

	// Prosledjivanje napona na h-most

}

void
robot_init() {
	encoder_init_all();
	hbridge_init();

	robot.phase = 0; // IDLE

	robot.Kp_pos = 3;
	robot.Kp_rot = 5;

	robot.x_ref = 0;
	robot.y_ref = 0;
	robot.theta_ref = 0;

	//odom init
	odom_init(&robot.odom, 0, 0, 0);

	// motor
	bdc_motors_init(
			&robot.mot1,
			10, 	// Kp
			0.1, 	// Kd
			1, 		// Ki
			1		// max speed
	);

	bdc_motors_init(
			&robot.mot2,
			10, 	// Kp
			0.1, 	// Kd
			1, 		// Ki
			1		// max speed m/s
	);

}

/* ---- STATIC fn ----------- */

static float
normalize_angle(float angle) {
	float tmp = angle;
	if (angle > M_PI)
		tmp -= 2 * M_PI;
	if (angle < - M_PI)
		tmp += 2 * M_PI;

	return tmp;
}
