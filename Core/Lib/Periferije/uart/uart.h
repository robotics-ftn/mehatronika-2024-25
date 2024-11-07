/*
 * uart.h
 *
 *  Created on: Oct 28, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_UART_UART_H_
#define LIB_PERIFERIJE_UART_UART_H_

#include <stdint.h>

void uart2_init();
void uart2_send_ch(uint8_t ch);
void uart2_send_string(const char *data);
void uart2_send_data(uint8_t *data, uint32_t size);

#endif /* LIB_PERIFERIJE_UART_UART_H_ */
