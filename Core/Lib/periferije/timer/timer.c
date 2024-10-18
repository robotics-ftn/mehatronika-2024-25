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

  TIM10->CR1 &= ~(0b1 << 2); // Šta pokreće reinicijalizaciju
  TIM10->CR1 &= ~(0b1 << 1); // Dozvola događaja

  // Vrednost takta: 84MHz
  // Željena vrednost: 1kHz - koliko brzo vremenski prekid da se izvršava
  TIM10->PSC = 83; // Delitelj frekvencije -> 1Mhz
  TIM10->ARR = 999; // Maksimalna vrednost brojanja -> 1kHz
}
