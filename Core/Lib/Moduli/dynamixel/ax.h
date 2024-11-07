/*
 * ax.h
 *
 *  Created on: Nov 4, 2024
 *      Author: dexter
 */

#ifndef LIB_MODULI_DYNAMIXEL_AX_H_
#define LIB_MODULI_DYNAMIXEL_AX_H_

#include <stdint.h>

void ax_uart1_init();
void ax_uart_send_byte(uint8_t byte);

void ax_led(uint8_t id, uint8_t isOn);
void ax_send_data(uint8_t id, uint8_t instruction, uint8_t *params, uint8_t param_size);

#endif /* LIB_MODULI_DYNAMIXEL_AX_H_ */

