/*
 * uart.c
 *
 *  Created on: Nov 7, 2024
 *      Author: Andrej Čilag
 */

#include "uart.h"
#include "stm32f4xx.h"

#define MAX_BUFFER 30

static void
uart6_init ();
//static void
//buffer_upis (uint8_t podatak);

static volatile uint8_t buffer[MAX_BUFFER];
static volatile uint8_t buffer_size = 0;
static volatile uint8_t upis_id = 0;
static volatile uint8_t citaj_id = 0;

void
uart_init ()
{
  uart6_init ();
  // uart2_inti ();
  // .....
}

static void
uart6_init ()
{
  // PA11 - TX
  // PA12 - RX
  RCC->AHB1ENR |= (0b1 << 0); // Ukljucivanje takta porta A

  const uint8_t TX_PIN = 11;
//  const uint8_t RX_PIN = 12;

  GPIOA->MODER |= (0b10 << TX_PIN * 2); // Pinovi podeseni da budu alternativna funkcija
//  GPIOA->MODER |= (0b10 << RX_PIN * 2); // Pinovi podeseni da budu alternativna funkcija

  // Half-duplex
  GPIOA->OTYPER |= (0b1 << TX_PIN); // Open-drain
//  GPIOA->PUPDR &= ~(0b11 << TX_PIN * 2);
  GPIOA->PUPDR |= (0b01 << TX_PIN * 2); // Pull-up

  const uint8_t AF = 8; // USART6

  GPIOA->AFR[TX_PIN / 8] |= (AF << (TX_PIN * 4) % 32);
//  GPIOA->AFR[RX_PIN / 8] |= (AF << (RX_PIN * 4) % 32);

  RCC->APB2ENR |= (0b1 << 5); // Ukljucivanje takta za usart6

  USART6->CR1 &= ~(0b1 << 12); // Duzina reci poruke je 8 bita
  USART6->CR1 &= ~(0b1 << 10); // Ne koristimo bit parnosti
  USART6->CR2 &= ~(0b11 << 12); // Koristimo 1 stop bit

  // Brzina komunikacije
  // Baudrate: 9600
  USART6->BRR |= (546 << 4) | (14 << 0);

  // Odobravanje slanja i prihvatanja podataka
  USART6->CR1 |= (0b1 << 2) | (0b1 << 3);
  USART6->CR1 |= (0b1 << 5); // Generisanje prekida prilikom pristizanja poruke

  // Half-duplex
  USART6->CR2 &= ~((0b1 << 11) | (0b1 << 14));
  USART6->CR3 &= ~((0b1 << 1) | (0b1 << 5));
  USART6->CR3 |= (0b1 << 3); // Ukljucivanje Half-duplex rezima

  const uint8_t PREKID_ID = 71;
  NVIC->ISER[PREKID_ID / 32] = (0b1 << PREKID_ID % 32);

  USART6->CR1 |= (0b1 << 13);
}

void
uart_send (uint8_t podatak)
{
  USART6->DR = podatak; // Slanje podatka
  // promenljiva = USART6->DR; //  Prihvatanje podatka

  while (!(USART6->SR & (0b1 << 6)))
    ; // Cekamo dok se podatak ne posalje
  USART6->SR &= ~(0b1 << 6);
}

void
uart_send_bytes (uint8_t *niz, uint8_t velicina_niza)
{
  for (uint8_t i = 0; i < velicina_niza; i++)
    {
      uart_send(niz[i]);
    }
}

void
uart_send_str (char *str)
{
  while (*str != '\0') // Saljemo sve dok ne dodjemo do kraja stringa
    {
      uart_send (*str);
      str++;
    }
}

void
uart_buffer_upis (uint8_t podatak)
{
  buffer[upis_id] = podatak;
//  upis_id++;
//  upis_id %= MAX_BUFFER;
  upis_id = (upis_id + 1) % MAX_BUFFER;

  if (buffer_size < MAX_BUFFER)
    {
      buffer_size++;
    }
  else
    {
//      citaj_id++;
      citaj_id = (citaj_id + 1) % MAX_BUFFER;
    }
}

uint8_t
uart_buffer_citaj ()
{
  uint8_t temp = 0;

  if (buffer_size > 0)
    {
      temp = buffer[citaj_id];
      citaj_id = (citaj_id + 1) % MAX_BUFFER;
      buffer_size--;
    }

  return temp;
}

//void
//USART6_IRQHandler ()
//{
//  if (USART6->SR & (0b1 << 5))
//    {
//      USART6->SR &= ~(0b1 << 5);
//
//      // Ocitavamo pristigli podatak
////      buffer[buffer_size++] = USART6->DR;
////      buffer_size++;
//      buffer_upis (USART6->DR);
//    }
//}
