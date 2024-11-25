/*
 * uart.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Andrej Čilag
 */

#include "uart.h"
#include "stm32f4xx.h"

#define VELICINA_BUFFERA 30

static void
uart6_init ();

//static void
//upisi_u_buffer (uint8_t podatak);
//static uint8_t
//citanje_iz_buffera ();

volatile static uint8_t buffer[VELICINA_BUFFERA];
volatile static uint8_t buffer_size = 0;
volatile static uint8_t index_upisa = 0;
volatile static uint8_t index_citanja = 0;

void
uart_init ()
{
  uart6_init ();
}

static void
uart6_init ()
{
  RCC->AHB1ENR |= (0b1 << 0); // Dozvola takta na portu A

  // Podesavanje Porta A
  // PA11 - TX // slanje podataka
  // PA12 - RX // prihvatanje podataka

  const uint8_t tx_pin = 11;
  const uint8_t rx_pin = 12;

  GPIOA->MODER |= (0b10 << tx_pin * 2);
//  GPIOA->MODER |= (0b10 << rx_pin * 2);

  // Half-duplex podesavanje pina
  GPIOA->OTYPER |= (0b1 << tx_pin);
  GPIOA->PUPDR |= (0b01 << tx_pin * 2);

  const uint8_t alternativna_funkcija = 8;

  GPIOA->AFR[tx_pin / 8] |= (alternativna_funkcija << (tx_pin % 8) * 4); // PA11
//  GPIOA->AFR[rx_pin / 8] |= (alternativna_funkcija << (rx_pin % 8) * 4); // PA12

  RCC->APB2ENR |= (0b1 << 5); // Dozvola takta za UART 6

  USART6->CR1 &= ~(0b1 << 12); // Duzina reci je 8 bita - 1 bajt
  USART6->CR2 &= ~(0b11 << 12); // Poruka ima 1 stop bit
  USART6->CR1 &= ~(0b1 << 10); // Iskljucujemo bit parnosti

  USART6->CR1 |= (0b1 << 2); // Ukljuvivanje RX
  USART6->CR1 |= (0b1 << 3); // Ukljuvivanje TX
  USART6->CR1 |= (0b1 << 5); // Dozvola prekida za pristizanje poruke

  // Podesavanje Baudrate-a => 9600
  USART6->BRR |= (546 << 4);
  USART6->BRR |= (14 << 0);

  // Podesavanje Half-duplex rezima
  USART6->CR2 &= ~(0b1 << 11);
  USART6->CR2 &= ~(0b1 << 14);
  USART6->CR3 &= ~(0b1 << 1);
  USART6->CR3 &= ~(0b1 << 5);
  USART6->CR3 |= (0b1 << 3);

  const uint8_t usart6_id = 71; // Pozicija prekida u okviru NVIC tabele
  NVIC->ISER[usart6_id / 32] |= (0b1 << usart6_id % 32); // Odabir prekidne rutine

  USART6->CR1 |= (0b1 << 13); // Ukljucivanje USART-a
}

void
uart_slanje (uint8_t podatak)
{
  USART6->DR = podatak; // Saljemo podatak
  while (!(USART6->SR & (0b1 << 6)))
    ; // Cekamo da podatak bude poslat
}

void
uart_slanje_bytes (uint8_t * niz, uint8_t niz_velicina)
{
  for (uint8_t i = 0; i < niz_velicina; i++)
    {
      uart_slanje(niz[i]);
    }
}

void
uart_slanje_str (char *str)
{
  while (*str != '\0')
    {
      uart_slanje (*str);
      *str++;
    }
}

void
upisi_u_buffer (uint8_t podatak)
{
  buffer[index_upisa] = podatak;
  index_upisa = (index_upisa + 1) % VELICINA_BUFFERA;

  if (buffer_size < VELICINA_BUFFERA)
    {
      buffer_size++;
    }
  else
    {
      index_citanja = (index_citanja + 1) % VELICINA_BUFFERA;
    }
}

uint8_t
citanje_iz_buffera ()
{
  uint8_t podatak;

  if (buffer_size > 0)
    {
      podatak = buffer[index_citanja];
      index_citanja = (index_citanja + 1) % VELICINA_BUFFERA;
      buffer_size--;
    }

  return podatak;
}

//void
//USART6_IRQHandler ()
//{
//  if (USART6->SR & (0b1 << 5))
//    {
//      USART6->SR &= ~(0b1 << 5); // Resetujemo bit kako bi prekid ponovo mogao da se izvrsi
//
////      buffer[buffer_size] = USART6->DR;
////      buffer_size++;
//      upisi_u_buffer(USART6->DR);
//    }
//}
