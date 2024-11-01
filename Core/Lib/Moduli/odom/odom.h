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
	float x;
	float y;
	float theta;

	float v;
	float w;
}odom_t;

void odom_init(float x, float y, float theta);
void odom_update(uint32_t dt_ms);

#endif /* LIB_MODULI_ODOM_ODOM_H_ */
