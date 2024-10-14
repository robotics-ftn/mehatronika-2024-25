/*
 * gpio.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Andrej Čilag
 */

#include "gpio.h"
#include "stm32f4xx.h"
// #include <stdint.h>

static void
porta_init ();

void
gpio_init ()
{
  porta_init ();
  // protc_init ();
  // ...
}

static void
porta_init ()
{
  // Inicijalizacija porta A
  // Inicijalizacija PA5 -> LED
  RCC->AHB1ENR |= (0b1 << 0); // Uključivanje takta

  const uint8_t pin = 5;

  GPIOA->MODER |= (0b1 << pin * 2); // PA5 - izlaz
  GPIOA->OTYPER &= ~(0b1 << pin); // PA5 - push-pull
  GPIOA->OSPEEDR &= ~(0b11 << pin * 2); // PA5 - brzina low
  GPIOA->PUPDR &= ~(0b11 << pin * 2); // PA5 - bez pullup/pulldown-a

}
