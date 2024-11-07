/*
 * ax.c
 *
 *  Created on: Nov 4, 2024
 *      Author: dexter
 */

#include "ax.h"
#include "Periferije/uart/uart.h"
#include "stm32f401xe.h"


#define AX_HEADER 		0xFF

#define AX_READ			0x02
#define AX_WRITE		0x03

#define AX_ID_Motor1	5
#define AX_ID_Motor2	6

#define AX_LED			25

static void ax_uart_send_byte(uint8_t byte);
static void ax_uart_send_arr(uint8_t *data, uint8_t size);


void
ax_led(uint8_t id, uint8_t isOn) {
	uint8_t params[] = {25, isOn};
	ax_send_data(id, AX_WRITE, params, 2);
}

void
ax_send_data(uint8_t id, uint8_t instruction, uint8_t *params, uint8_t param_size) {
	const uint8_t len = param_size + 2;
	uint16_t checksum = id + len + instruction;

	/* [ 0xFF | 0xFF | Packet ID | Length | Instruction | Param 1 | … | Param N | CHKSUM ]*/

	ax_uart_send_byte(AX_HEADER);
	ax_uart_send_byte(AX_HEADER);
	ax_uart_send_byte(id);
	ax_uart_send_byte(len);
	ax_uart_send_byte(instruction);

	for (int i = 0; i < param_size; i++) {
		checksum += *(params + i);
	}

	ax_uart_send_arr(params, param_size);

	checksum = ~(checksum & 0xFF);

	ax_uart_send_byte((uint8_t)checksum);
}

void
ax_uart1_init() {
	// Enable clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/*
	 * PA9 --> USART1_TX (AF7)
	 */

	GPIOA->MODER &= ~(0b11 << 9 * 2);
	GPIOA->MODER |=  (0b10 << 9 * 2);

	GPIOA->AFR[1] &= ~(0b1111 << 1 * 4);
	GPIOA->AFR[1] |=  (7 << 1 * 4);

	/*
	 * the TX pin is always released when no data is transmitted.
	 * Thus, it acts as a standard I/O in idle or in reception.
	 * It means that the I/O must be configured so that TX is
	 * configured as floating input (or output high open-drain)
	 * when not driven by the USART.
	 */
	GPIOA->OTYPER |= (0b1 << 9); // open-drain
	GPIOA->PUPDR  &= ~(0b11 << 9 * 2);
	GPIOA->PUPDR  |=  (0b01 << 9 * 2); // pull-up

	/*************************************
	 * 				USART 1 config
	 *************************************/
	USART1->CR1 &= ~(0b1 << 15); // oversampling by 16

	// 1 start, 8 data, n stop bits
	USART1->CR1 &= ~(0b1 << 12);

	// bit parnosti
	USART1->CR1 &= ~(0b1 << 10);

	//RX enable prekid
	USART1->CR1 |= (0b1 << 5);

	// Transmiter enable
	USART1->CR1 |= (0b1 << 3);
	// Receiver enable
	USART1->CR1 |= (0b1 << 2);

	// 1 stop bit
	USART1->CR2 &= ~(0b11 << 12);

	// half duplex turn off
	//	USART2->CR3 &= ~(0b1 << 3);
	//enable half duplex
	USART1->CR3 |=  (0b1 << 3);

	// 115200 baudrate
	uint32_t div_mantissa = 45;
	uint32_t div_fraction = 9;

	USART1->BRR = (div_mantissa << 4 | div_fraction);
	USART1->CR1 |= (0b1 << 13); // enable USART

	// enable prekid u NVIC
//	NVIC->ISER[(int)(38/32)] |= (0b1 << 38 % 32);
}

static void
ax_uart_send_byte(uint8_t byte) {
	while( !( (USART1->SR & (0b1 << 6)) == (0b1 << 6)));
	USART1->DR = byte;
}

static void
ax_uart_send_arr(uint8_t *data, uint8_t size) {
	for (int i = 0; i < size; i++)
		ax_uart_send_byte(*data++);
}
