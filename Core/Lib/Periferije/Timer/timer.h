/*
 * timer.h
 *
 *  Created on: Oct 14, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_TIMER_TIMER_H_
#define LIB_PERIFERIJE_TIMER_TIMER_H_

#include "stm32f401xe.h"

typedef struct{
	uint8_t flg_100s 	: 1;
	uint8_t flg_85s 	: 1;
	uint8_t flg_1s		: 1;

	uint8_t flg_timeout_start : 1;
	uint8_t flg_timeout_end	  : 1;
}sTimerFLags_t;

volatile extern uint32_t system_ms;
volatile extern sTimerFLags_t timer_flags;

void TIM1_Init();

#endif /* LIB_PERIFERIJE_TIMER_TIMER_H_ */
