/*
 * uart.h
 *
 *  Created on: Oct 31, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_UART_UART_H_
#define LIB_PERIFERIJE_UART_UART_H_

void uart2_init();
void uart2_send_data(unsigned char data);
void uart2_send_data_array(unsigned char *data, unsigned int size);

void uart2_echo();

#endif /* LIB_PERIFERIJE_UART_UART_H_ */
