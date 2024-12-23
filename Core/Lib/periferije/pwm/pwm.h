/*
 * pwm.h
 *
 *  Created on: Nov 18, 2024
 *      Author: Andrej Čilag
 */

#ifndef LIB_PERIFERIJE_PWM_PWM_H_
#define LIB_PERIFERIJE_PWM_PWM_H_

#include <stdint.h>

void
pwm_init ();

void
motor1_init ();

void
pwm_rc_dc (uint8_t dc);

#endif /* LIB_PERIFERIJE_PWM_PWM_H_ */
