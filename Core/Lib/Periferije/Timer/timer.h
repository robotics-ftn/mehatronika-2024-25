/*
 * timer.h
 *
 *  Created on: Oct 17, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_TIMER_TIMER_H_
#define LIB_PERIFERIJE_TIMER_TIMER_H_

#include "stm32f4xx.h"

typedef struct
{
	uint8_t flg_100s 	: 1;
	uint8_t flg_85s		: 1;

	uint8_t flg_timeout_start : 1;
	uint8_t flg_timeout_end	  : 1;

}timer_flgs_t;

extern timer_flgs_t timer_flgs;

void TIM1_Init();
void TIM1_Enable();
void TIM1_Disable();
void TIM1_timeout(uint32_t ms);

#endif /* LIB_PERIFERIJE_TIMER_TIMER_H_ */
