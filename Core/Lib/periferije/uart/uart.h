/*
 * uart.h
 *
 *  Created on: Oct 28, 2024
 *      Author: Andrej Čilag
 */

#ifndef LIB_PERIFERIJE_UART_UART_H_
#define LIB_PERIFERIJE_UART_UART_H_

#include <stdint.h>

void
uart_init ();

void
uart_slanje (uint8_t podatak);

void
uart_slanje_bytes (uint8_t * niz, uint8_t niz_velicina);

void
uart_slanje_str (char * str);

void
upisi_u_buffer (uint8_t podatak);
uint8_t
citanje_iz_buffera ();

#endif /* LIB_PERIFERIJE_UART_UART_H_ */
