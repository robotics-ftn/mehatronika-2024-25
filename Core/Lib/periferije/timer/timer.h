/*
 * timer.h
 *
 *  Created on: Oct 17, 2024
 *      Author: Andrej Čilag
 */

#ifndef LIB_PERIFERIJE_TIMER_TIMER_H_
#define LIB_PERIFERIJE_TIMER_TIMER_H_

#include <stdbool.h>
#include <stdint.h>

void
timer_init ();

bool
timer_delay (uint32_t ms);

#endif /* LIB_PERIFERIJE_TIMER_TIMER_H_ */
