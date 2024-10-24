/*
 * encoder.c
 *
 *  Created on: Oct 24, 2024
 *      Author: Andrej Čilag
 */

#include "encoder.h"
#include "stm32f4xx.h"

static void
encoder1_init ();

void
encoder_init ()
{
  encoder1_init ();
  // encoder2_init();
}

static void
encoder1_init ()
{
  // Inicijalizacija GPIO
  // PA15, PB3
  RCC->AHB1ENR |= (0b1 << 0) | (0b1 << 1); // Takt za port A i B

  GPIOA->MODER |= (0b10 << 15 * 2); // Alternativna funkcija
  GPIOB->MODER |= (0b10 << 3 * 2); // Alternativna funkcija

  GPIOA->AFR[1] |= (0b0001 << 28); // Alternativna funkcija TIM2
  GPIOB->AFR[0] |= (0b0001 << 12); // Alternativna funkcija TIM2

  // Inicijalizacija tajmera
  RCC->APB1ENR |= (0b1 << 0); // Takt za TIM2

}
