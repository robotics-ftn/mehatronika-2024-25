/*
 * odom.c
 *
 *  Created on: Oct 21, 2024
 *      Author: dexter
 */

#include <math.h>
#include <stdlib.h>
#include "odom.h"
#include "Moduli/Encoder/encoder.h"

static odom_t* odom;
static const float d = 0.07; // poluprecnik tocka [m]
static const float L = 0.22; // razmak izmedju tockova [m]

static const float inc2rad = 0.000766616; // 2 x PI / (4x2048)


void
odom_init(float x, float y, float theta) {
	odom = malloc(sizeof(odom));
	odom->x = x;
	odom->y = y;
	odom->theta = theta;

	odom->v = 0;
	odom->w = 0;
}

void
odom_update(uint16_t dt_ms) {
	float dt_s, v_l, v_d;
	int32_t v_l_inc, v_d_inc;

	dt_s = dt_ms / 1000.0;
	v_l_inc = encoder1_inc_delta();
	v_d_inc = v_l_inc;

	v_l = v_l_inc * inc2rad * d / dt_s;
	v_d = v_d_inc * inc2rad * d / dt_s;

	odom->v = (v_d + v_l) / 2.0;
	odom->w = (v_d - v_l) / L;

	odom->x += odom->v * dt_s * cos(odom->theta + odom->w * dt_s / 2.0 );
	odom->y += odom->v * dt_s * sin(odom->theta + odom->w * dt_s / 2.0 );
	odom->theta += odom->w * dt_s;

	if (odom->theta > M_PI)
		odom->theta -= 2*M_PI;
	else if (odom->theta < - M_PI)
		odom->theta += 2*M_PI;

}
