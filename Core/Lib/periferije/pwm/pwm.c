/*
 * pwm.c
 *
 *  Created on: Nov 18, 2024
 *      Author: Andrej Čilag
 */

#include "pwm.h"
#include "stm32f4xx.h"

static void
tim11_init ();

void
pwm_init ()
{
  tim11_init ();
}

static void
tim11_init ()
{
  // TIM11 -> PB9
  RCC->AHB1ENR |= (0b1 << 1); // Ukljucivanje takta za port B

  const uint8_t PWM_PIN = 9;
  GPIOB->MODER |= (0b10 << PWM_PIN * 2); // Pin je alternativna funkcija

  const uint8_t AF = 3;
  GPIOB->AFR[PWM_PIN / 8] |= (AF << (PWM_PIN * 4) % 32);

  RCC->APB2ENR |= (0b1 << 18); // Ukljucivanje takta za TIM11

  // fclk = 84MHz -> signal: 50Hz
  TIM11->PSC = 84 - 1;
  TIM11->ARR = 20000 - 1;

  // PWM1 mode - neinvertujemo signal!
  TIM11->CCMR1 |= (0b110 << 4);
}
