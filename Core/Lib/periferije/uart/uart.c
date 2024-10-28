/*
 * uart.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Andrej Čilag
 */

#include "uart.h"
#include "stm32f4xx.h"

static void
uart6_init ();

void
uart_init ()
{
  uart6_init ();
}

static void
uart6_init ()
{
  RCC->AHB1ENR |= (0b1 << 0); // Dozvola takta na portu A
  RCC->APB2ENR |= (0b1 << 5); // Dozvola takta za UART 6

  // Podesavanje Porta A
  // PA11 - TX // slanje podataka
  // PA12 - RX // prihvatanje podataka

  const uint8_t tx_pin = 11;
  const uint8_t rx_pin = 12;

  GPIOA->MODER |= (0b10 << tx_pin * 2);
  GPIOA->MODER |= (0b10 << rx_pin * 2);

  const uint8_t alternativna_funkcija = 8;

  GPIOA->AFR(1) |= (alternativna_funkcija << 12); // PA11
  GPIOA->AFR(1) |= (alternativna_funkcija << 16); // PA12
}
