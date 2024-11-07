/*
 * ax.c
 *
 *  Created on: Nov 7, 2024
 *      Author: dexter
 */

#include "ax.h"

static void ax_send_byte(uint8_t data);
static void ax_send_array(uint8_t *data, uint8_t size);


void
ax_goal_position(uint8_t id, float angle){
	angle = (angle < 0) ? 0 :( angle > 300 ? 300 : angle);
	uint16_t angle_inc = (uint16_t)((angle * 1024/300.0) - 1);
	uint8_t params[] = {
			30,
			(uint8_t)(angle_inc & 0xFF),
			(uint8_t)((angle_inc >> 8) & 0xFF),
	};

	ax_send_command(id, AX_WRITE, params, 3);
}

void
ax_led(uint8_t id, uint8_t state) {
	uint8_t params[] = {25, state};
	ax_send_command(id, AX_WRITE, params, 2);
}

void
ax_send_command(uint8_t id, uint8_t instruction, uint8_t *params, uint8_t n_params) {
	const uint8_t len = n_params + 2;
	uint16_t checksum = id + instruction + len;

	for (int i = 0; i < n_params; i ++)
		checksum += *(params + i);

	checksum = ~(checksum & 0xFF);

	ax_send_byte(AX_HEADER);
	ax_send_byte(AX_HEADER);
	ax_send_byte(id);
	ax_send_byte(len);
	ax_send_byte(instruction);
	ax_send_array(params, n_params);
	ax_send_byte(checksum);
}

void
ax_uart_init() {

	/*
	 * PA9 USART1 TX (AF7)
	 * Baudrate: 115200
	 */

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/* Init GPIO 						*/
	GPIOA->MODER &= ~(0b11 << 9 * 2);
	GPIOA->MODER |=  (0b10 << 9 * 2);

	// Biramo alt. fun., nama treba AF7
	GPIOA->AFR[1] &= ~(0b1111 << 1 * 4);
	GPIOA->AFR[1] |=  (		7 << 1 * 4);

	/* Open-drain, pull-up otpornik */
	GPIOA->OTYPER |= (0b1 << 9); // open-drain
	GPIOA->PUPDR  &= ~(0b11 << 9 * 2);
	GPIOA->PUPDR  |=  (0b01 << 9 * 2); // pull-up otpornik

	/* Init UART1 						*/

	// OVER8 off = 16 samples
	USART1->CR1 &= ~(0b1 << 15);
	// 1 start, 8 data, n Stop bits
	USART1->CR1 &= ~(0b1 << 12);
	// iskljuci bit parnosti
	USART1->CR1 &= ~(0b1 << 10);
	// Ukljuci prekid na RX
	//	USART1->CR1 |= (0b1 << 5);

	// enable TX and RX
	USART1->CR1 |= (0b1 << 3);
	USART1->CR1 |= (0b1 << 2);

	// enable half duplex
	USART1->CR3 |= (0b1 << 3);

	// Definisanje baudrate-a

	/*
	 * Desired baudrate = 115200
	 *
	 * 			f_clk
	 * BR = -------------
	 * 		16 x USARTDIV
	 *
	 * 				 f_clk			84 MHz
	 * USARTDIV = ------------- = ---------- = 45.57
	 * 				16 x BR		  16 x 115200
	 *
	 * DIV_Fraction = 16 * 0.57 = 9.12 ~~ 9
	 * DIV_Mantissa = 45
	 */

	USART1->BRR = (45 << 4 | 9);

	// Enable USART
	USART1->CR1 |= (0b1 << 13);
}


static void
ax_send_byte(uint8_t data) {
	while (!((USART1->SR & USART_SR_TC) == USART_SR_TC));
	USART1->DR = data;
}

static void
ax_send_array(uint8_t *data, uint8_t size) {
	for (int i = 0; i < size; i ++)
		ax_send_byte(*data++);
}


