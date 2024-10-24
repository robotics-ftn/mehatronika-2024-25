/*
 * odom.c
 *
 *  Created on: Oct 24, 2024
 *      Author: dexter
 */

#include "odom.h"
#include "Periferije/encoder/encoder.h"
#include <math.h>

const float
L = 225, 				// razmak izmedju tockova enkodera
r = 35,	 				// poluprecnik enkoderskih tockova
inc2rad = 0.00076699; 	// pretvaranje inkremenata enkodera u radijane

static odom_t odom;

static float
normalize_angle(float angle);

void
odom_init(float x, float y, float theta) {
	odom.x = x;
	odom.y = y;
	odom.theta = theta;
}

void
odom_update(uint16_t dt_ms) {
	int32_t v_l_inc;
	int32_t v_d_inc;
	float v_l, v_d, dt_s;

	v_l_inc = encoder_desni_get_inc();
	v_d_inc = encoder_desni_get_inc();

	dt_s = dt_ms / 1000.0;

	v_l = v_l_inc * inc2rad * r / dt_s;
	v_d = v_d_inc * inc2rad * r / dt_s;


	odom.v = (v_d + v_l) / 2.0;
	odom.w = (v_d - v_l) / L;

	odom.x += odom.v * cos(odom.theta + odom.w * dt_s / 2.0) * dt_s;
	odom.y += odom.v * sin(odom.theta + odom.w * dt_s / 2.0) * dt_s;

	odom.theta += odom.w * dt_s;
	odom.theta = normalize_angle(odom.theta);
}

static float
normalize_angle(float angle) {
	if (angle > M_PI)
		angle -= 2 * M_PI;
	if (angle < - M_PI)
		angle += 2 * M_PI;

	return angle;
}

