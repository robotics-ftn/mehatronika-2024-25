/*
 * odom.c
 *
 *  Created on: Oct 21, 2024
 *      Author: dexter
 */

#include <math.h>
#include "odom.h"

static odom_t* odom;
static const float R = 0.07; // precnik tocka
static const float L = 0.22; // razmak izmedju tockova

static const float inc2rad = 0.000766616; // 2 x PI / (4x2048)

void
odom_init(float x, float y, float theta) {
	odom->x = x;
	odom->y = y;
	odom->theta = theta;

	odom->v = 0;
	odom->w = 0;
}

void
odom_update(int32_t v_d, int32_t v_l, uint16_t dt_ms) {

	float dt_s = dt_ms / 1000;
	float vd = v_d * inc2rad * 1000 / dt_ms; // rad / s
	float vl = v_l * inc2rad * 1000 / dt_ms; // rad / s

	odom->v = (vd + vl) / 2.0;
	odom->w = (vd - vl) / L;

	odom->x += odom->v * cos(odom->theta + odom->w * dt_s / 2.0 );
	odom->y += odom->v * sin(odom->theta + odom->w * dt_s / 2.0 );
	odom->theta += odom->w * dt_s;

	if (odom->theta > M_PI)
		odom->theta -= 2*M_PI;
	else if (odom->theta < - M_PI)
		odom->theta += 2*M_PI;

}
