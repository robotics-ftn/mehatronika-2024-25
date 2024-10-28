/*
 * odom.h
 *
 *  Created on: Oct 21, 2024
 *      Author: dexter
 */

#ifndef LIB_MODULI_ODOMETRIJA_ODOM_H_
#define LIB_MODULI_ODOMETRIJA_ODOM_H_

#include <stdint.h>

typedef struct {
	float x;
	float y;
	float theta;

	float v;
	float w;
}odom_t;

void odom_init(float x, float y, float theta);
void odom_update(uint16_t dt_ms);

#endif /* LIB_MODULI_ODOMETRIJA_ODOM_H_ */
