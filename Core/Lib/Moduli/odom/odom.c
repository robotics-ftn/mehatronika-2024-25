/*
 * odom.c
 *
 *  Created on: Oct 24, 2024
 *      Author: dexter
 */

#include "odom.h"
#include <stdlib.h>

void
odom_init(odom_t* odom, float x, float y, float theta) {
	odom->x = x;
	odom->y = y;
	odom->theta = theta;
}




