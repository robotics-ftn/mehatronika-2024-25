/*
 * timer.c
 *
 *  Created on: Oct 17, 2024
 *      Author: Andrej Čilag
 */

#include "timer.h"
#include "stm32f4xx.h"

static void
tim10_init ();

void
timer_init ()
{
  tim10_init ();
  // tim2_init ();
  // ...
}

static void
tim10_init ()
{
  RCC->APB2ENR |= (0b1 << 17); // Uključivanje takta za timer 10

  TIM10->CR1 &= ~(0b1 << 2);
  TIM10->CR1 &= ~(0b1 << 1);

  TIM10->PSC = 83;
  TIM10->ARR = 999;
}
