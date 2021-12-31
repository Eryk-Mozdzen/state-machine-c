/*
 * rc5_decoder.h
 *
 *  Created on: Dec 30, 2021
 *      Author: ermoz
 */

#ifndef INC_RC5_DECODER_H_
#define INC_RC5_DECODER_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef enum {
	RC5_STATE_START1,
	RC5_STATE_MID1,
	RC5_STATE_START0,
	RC5_STATE_MID0
} RC5_State_t;

typedef union {
	struct {
		uint8_t toggle : 1;
		uint8_t address : 5;
		uint8_t command : 6;
	};

	uint16_t frame;
} RC5_Message_t;

typedef struct {
	uint8_t bits_ready;
	RC5_Message_t message;

	TIM_HandleTypeDef htim;
} RC5_FSM_Data_t;

void rc5_emit1(void *);
void rc5_emit0(void *);

#endif
