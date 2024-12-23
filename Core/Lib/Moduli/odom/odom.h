/*
 * odom.h
 *
 *  Created on: Oct 24, 2024
 *      Author: dexter
 */

#ifndef LIB_MODULI_ODOM_ODOM_H_
#define LIB_MODULI_ODOM_ODOM_H_

#include <stdint.h>

typedef struct
{
	double x;
	double y;
	double theta;

	double v;
	double w;
}odom_t;

void odom_init(odom_t* odom, float x, float y, float theta);

#endif /* LIB_MODULI_ODOM_ODOM_H_ */
