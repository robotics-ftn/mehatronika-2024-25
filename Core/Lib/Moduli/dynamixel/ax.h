/*
 * ax.h
 *
 *  Created on: Nov 7, 2024
 *      Author: dexter
 */

#ifndef LIB_MODULI_DYNAMIXEL_AX_H_
#define LIB_MODULI_DYNAMIXEL_AX_H_

#include "stm32f401xe.h"


#define AX_HEADER	0xFF

#define AX_READ		0x02
#define AX_WRITE	0x03

#define AX_MOTOR1	5
#define AX_MOTOR2	6


void ax_set_position_and_speed(uint8_t id, float angle, float rpm);
void ax_moving_speed(uint8_t id, float rpm);
void ax_goal_position(uint8_t id, float angle);
void ax_led(uint8_t id, uint8_t state);
void ax_send_command(uint8_t id, uint8_t instruction, uint8_t *params, uint8_t n_params);
void ax_uart_init();

#endif /* LIB_MODULI_DYNAMIXEL_AX_H_ */
