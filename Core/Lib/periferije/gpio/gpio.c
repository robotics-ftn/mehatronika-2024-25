/*
 * gpio.c
 *
 *  Created on: Oct 17, 2024
 *      Author: Andrej Čilag
 */

#include "gpio.h"
#include "stm32f4xx.h"

static void
porta_init ();

void
gpio_init ()
{
  porta_init ();
  // portb_init ();
  // portc_init ();
  // ...
}

static void
porta_init ()
{
  RCC->AHB1ENR |= (0b1 << 0); // Dozvola takta na portu A

  const uint8_t led_pin = LED_PIN;

  GPIOA->MODER |= (0b01 << led_pin * 2); // Tip pina: izlaz
  GPIOA->OTYPER &= ~(0b1 << led_pin); // Push-pull
  GPIOA->OSPEEDR &= ~(0b11 << led_pin * 2); // Niska brzina
  GPIOA->PUPDR &= ~(0b11 << led_pin * 2); // Nije pull-up ni pull-down
}
