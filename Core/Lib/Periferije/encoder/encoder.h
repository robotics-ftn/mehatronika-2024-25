/*
 * encoder.h
 *
 *  Created on: Oct 24, 2024
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_ENCODER_ENCODER_H_
#define LIB_PERIFERIJE_ENCODER_ENCODER_H_

#include <stdint.h>

void	encoder_init_all();

void 	encoder_desni_init();
int32_t encoder_desni_get_inc();

void 	encoder_levi_init();
int32_t encoder_levi_get_inc();

#endif /* LIB_PERIFERIJE_ENCODER_ENCODER_H_ */
