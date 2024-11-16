/*
 * pwm.c
 *
 *  Created on: Nov 14, 2024
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
  // PB9 - TIM11_CH1
  RCC->AHB1ENR |= (0b1 << 1); // Dozvola takta za port B

  const uint8_t PWM_PIN = 9;

  GPIOB->MODER |= (0b10 << PWM_PIN * 2); // Alternativna funkcija

  const uint8_t AF = 3; // Alternativna funkcija 3

  GPIOB->AFR[PWM_PIN / 8] |= (AF << (PWM_PIN * 4) % 32);


}
