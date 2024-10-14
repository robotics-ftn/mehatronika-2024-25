/*
 * timer.c
 *
 *  Created on: Oct 14, 2024
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
}

static void
tim10_init ()
{
  RCC->APB2ENR |= (0b1 << 17); // Uključivanje takta za tajmer 10

  TIM10->CR1 &= ~(0b1 << 2); // Šta može da resetuje (reinicijalizuje) tajmer
  TIM10->CR1 &= ~(0b1 << 1); // Dozvola događaja

  // Željena vrednost takta: 1kHz
  TIM10->PSC = 83; // 1MHz
  TIM10->ARR = 999; // 1kHz

  TIM10->DIER |= (0b1 << 0); // Dozvola prekida

  TIM10->EGR |= (0b1 << 0); // Resetovanje (reinicijalizacija) tajmera

  while (!(TIM10->SR & (0b1 << 0))); // Čekanje da se izvrši reinicijalizacija tajmera
  TIM10->SR &= ~(0b1 << 0); // Resetovanje statusnog bita

  TIM10->CR1 |= (0b1 << 2); // "Zabrana" resetovanja (reinicijalizacije) tajmera
}
