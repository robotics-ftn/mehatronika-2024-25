/*
 * uart.c
 *
 *  Created on: Oct 28, 2024
 *      Author: dexter
 */

#include "uart.h"
#include "stm32f401xe.h"

#define RB_SIZE 128

static uint32_t data[RB_SIZE];
static uint16_t read;
static uint16_t write;


void
uart2_init() {

	// Enable clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/*
	 * PA2 --> USART2_TX (AF7)
	 * PA3 --> USART2_RX (AF7)
	 */

	GPIOA->MODER &= ~(0b11 << 2 * 2 | 0b11 << 3 * 2);
	GPIOA->MODER |=  (0b10 << 2 * 2 | 0b10 << 3 * 2);

	GPIOA->AFR[0] &= ~(0b1111 << 2 * 4 | 0b1111 << 3 * 4);
	GPIOA->AFR[0] |=  (7 << 2 * 4 | 7 << 3 * 4);


	USART2->CR1 &= ~(0b1 << 15); // oversampling by 16

	// 1 start, 8 data, n stop bits
	USART2->CR1 &= ~(0b1 << 12);

	// bit parnosti
	USART2->CR1 &= ~(0b1 << 10);

	//RX enable prekid
	USART2->CR1 |= (0b1 << 5);

	// Transmiter enable
	USART2->CR1 |= (0b1 << 3);
	// Receiver enable
	USART2->CR1 |= (0b1 << 2);

	// 1 stop bit
	USART2->CR2 &= ~(0b11 << 12);

	// half duplex turn off
	USART2->CR3 &= ~(0b1 << 3);


	// USARTDIV = 273.4375
	// div_fraction = 16 * 0.4375 = 7
	uint32_t div_fraction = 7;
	uint32_t div_mantissa = 273;


	USART2->BRR = (div_mantissa << 4 | div_fraction);

	USART2->CR1 |= (0b1 << 13); // enable USART

	// enable prekid u NVIC
	NVIC->ISER[(int)(38/32)] |= (0b1 << 38 % 32);


	read = 0;
	write = 0;
}


void
uart2_send_ch(uint8_t ch) {
	// cekamo da se zavrsi slanje ukoliko postoji
	while( !( (USART2->SR & (0b1 << 6)) == (0b1 << 6)));
	USART2->DR = ch;
}

void
uart2_send_string(const char *data) {
	while (*data != '\0')
	{
		uart2_send_ch(*data);
		data++;
	}
}

void
USART2_IRQHandler() {

	if ((USART2->SR & (0b1 << 5)) == (0b1 << 5)) {

		data[write] = USART2->DR;
		write = (write + 1) % RB_SIZE;
		USART2->SR &= ~(0b1 << 5);

	}

}
