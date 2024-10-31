/*
 * uart.c
 *
 *  Created on: Oct 31, 2024
 *      Author: dexter
 */

#include "uart.h"
#include "stm32f401xe.h"

#define BUFFER_SIZE 64

static uint8_t ring_buffer[BUFFER_SIZE];
static uint8_t rb_write;
static uint8_t rb_read;

void
uart2_init() {

	/*
	 * PA2 --> UART2 TX AF7
	 * PA3 --> UART2 RX AF7
	 */

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/* Init GPIO 						*/
	GPIOA->MODER &= ~(0b11 << 2 * 2 | 0b11 << 3 * 2);
	GPIOA->MODER |=  (0b10 << 2 * 2 | 0b10 << 3 * 2);

	// Biramo alt. fun., nama treba AF7
	GPIOA->AFR[0] &= ~(0b1111 << 2 * 4 	| 0b1111 << 3 * 4);
	GPIOA->AFR[0] |=  (		7 << 2 * 4 	| 	   7 << 3 * 4);

	/* Init UART2 						*/

	// OVER8 off = 16 samples
	USART2->CR1 &= ~(0b1 << 15);
	// 1 start, 8 data, n Stop bits
	USART2->CR1 &= ~(0b1 << 12);
	// iskljuci bit parnosti
	USART2->CR1 &= ~(0b1 << 10);
	// Ukljuci prekid na RX
	USART2->CR1 |= (0b1 << 5);

	// enable TX and RX
	USART2->CR1 |= (0b1 << 3);
	USART2->CR1 |= (0b1 << 2);

	// disable half duplex
	USART2->CR3 &= ~(0b1 << 3);

	// Definisanje baudrate-a

	/*
	 * Desired baudrate = 9600
	 *
	 * 			f_clk
	 * BR = -------------
	 * 		16 x USARTDIV
	 *
	 * 				 f_clk			42 MHz
	 * USARTDIV = ------------- = ---------- = 273.4375
	 * 				16 x BR		  16 x 9600
	 *
	 * DIV_Fraction = 16 * 0.4375 = 7
	 * DIV_Mantissa = 273
	 */

	USART2->BRR = (273 << 4 | 7);

	// Enable USART
	USART2->CR1 |= (0b1 << 13);

	NVIC->ISER[(int)(38/32)] |= (0b1 << (38 % 32));
}

void
uart2_send_data(unsigned char data) {
	while (!((USART2->SR & 0b1 << 6) == (0b1 << 6)));
	USART2->DR = data;
}

void
uart2_echo() {
	if (rb_read == rb_write)
		return;

	while(ring_buffer[rb_read] != '\0') {
		uart2_send_data(ring_buffer[rb_read]);

		rb_read = (rb_read + 1) % BUFFER_SIZE;
	}

}

void
uart2_send_data_array(unsigned char *data, unsigned int size) {

	for (int i = 0; i < size; i ++) {
		uart2_send_data(*data);
		data++;
	}
}


void
USART2_IRQHandler() {
	// Check if RX generated IRQ
	if ( (USART2->SR & 0b1 << 5) == (0b1 << 5)) {

		ring_buffer[rb_write] = USART2->DR;
		rb_write = (rb_write + 1 ) % BUFFER_SIZE;

		ring_buffer[rb_write] = '\0'; // ovo je opciono

		// Clear RX bit
//		USART2->SR &= ~(0b1 << 5);
	}
}
