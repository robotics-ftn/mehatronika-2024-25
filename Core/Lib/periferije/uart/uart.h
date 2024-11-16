/*
 * uart.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Andrej Čilag
 */

#ifndef LIB_PERIFERIJE_UART_UART_H_
#define LIB_PERIFERIJE_UART_UART_H_

#include <stdint.h>

void
uart_init ();

void
uart_send (uint8_t podatak);

void
uart_send_str (char * str);

uint8_t
uart_buffer_citaj ();
void
uart_buffer_upis (uint8_t podatak);

void
uart_send_bytes (uint8_t *niz, uint8_t velicina_niza);

#endif /* LIB_PERIFERIJE_UART_UART_H_ */
