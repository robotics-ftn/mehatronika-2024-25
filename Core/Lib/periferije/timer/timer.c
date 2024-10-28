/*
 * timer.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Andrej Čilag
 */

#include "timer.h"
#include "stm32f4xx.h"
#include "tim.h"

// Privatni prototipovi
static void
tim10_init ();

// Promenljive
volatile int16_t brzina;
volatile uint16_t enc;

volatile uint32_t sys_time;

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

  const uint8_t tim10_prekid = 25;
  NVIC->ISER[tim10_prekid/32] |= (0b1 << (tim10_prekid % 32));

  TIM10->CR1 |= (0b1 << 0); // Uključujemo brojač/tajmer
}

bool
timer_delay (uint32_t ms) // ms - milisekunde
{
  static uint32_t start;
  static bool flag = true;

  if (flag)
    {
      start = sys_time;
      flag = false;
    }

  if ((start + ms) > sys_time)
    {
      return false;
    }

  flag = true;
  return true;
}

//void
//TIM1_UP_TIM10_IRQHandler ()
//{
//  if (TIM10->SR & (0b1 << 0))
//    {
//      TIM10->SR &= ~(0b1 << 0); // Samostalno se postavlja na jedinicu. Mi moramo da ga nuliramo kako bi naredni prekid mogao da se izvrši.
//
//      // 1ms
//      sys_time++;
//
//      enc = TIM2->CNT;
//
//      brzina = tim_brzina_1();
//    }
//}
