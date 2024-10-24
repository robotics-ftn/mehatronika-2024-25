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

volatile uint32_t sys_time = 0;

void
timer_init ()
{
  tim10_init ();
  // tim2_init ();
  // ...
}

bool
timer_delay (uint32_t ms)
{
  static uint32_t start = 0;

  if (start == 0)
    {
      start = sys_time;
    }

  if ((start + ms) <= sys_time)
    {
      start = 0;
      return true;
    }

  return false;
}

static void
tim10_init ()
{
  RCC->APB2ENR |= (0b1 << 17); // Uključivanje takta za timer 10

  TIM10->CR1 &= ~(0b1 << 2); // Šta pokreće reinicijalizaciju
  TIM10->CR1 &= ~(0b1 << 1); // Dozvola događaja

  TIM10->DIER |= (0b1 << 0); // Dozvola prekida

  // Vrednost takta: 84MHz
  // Željena vrednost: 1kHz - koliko brzo vremenski prekid da se izvršava
  TIM10->PSC = 83; // Delitelj frekvencije -> 1Mhz
  TIM10->ARR = 999; // Maksimalna vrednost brojanja -> 1kHz

  TIM10->EGR |= (0b1 << 0); // Reinicijalizuj tajmer (CNT postavljamo na početno stanje)
  while (!(TIM10->SR & (0b1 << 0)))
    {
      // prazno!
    }
  TIM10->SR &= ~(0b1 << 0); // Statusni bit postavljamo na nula da bi se tajmer ponovo mogao reinicijalizovati
  TIM10->CR1 |= (0b1 << 2); // Isključujemo mogućnost da "ručno" reinicijalizujemo tajmer (kako ga ne bi slučajno reinicijalizovali)

  const uint8_t tim10_id = 25;

  NVIC->ISER[tim10_id/32] |= (0b1 << tim10_id % 32);

  TIM10->CR1 |= (0b1 << 0); // Uključujemo tajmer
}

//void
//TIM1_UP_TIM10_IRQHandler ()
//{
//  if (TIM10->SR & (0b1 << 0))
//    {
//      TIM10->SR &= ~(0b1 << 0);
//
//      // Telo prekida...
//      sys_time++;
//
//    }
//
////  if (TIM1->SR & (0b1 << 0))
////    {
////      TIM1->SR &= ~(0b1 << 0);
////
////      // Telo prekida...
////    }
//}
