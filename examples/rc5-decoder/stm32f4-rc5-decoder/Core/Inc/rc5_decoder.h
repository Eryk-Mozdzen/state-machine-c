/*
 * rc5_decoder.h
 *
 *  Created on: Dec 30, 2021
 *      Author: ermoz
 */

#ifndef INC_RC5_DECODER_H_
#define INC_RC5_DECODER_H_

#include <stdlib.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

#define RC5_TIME_SHORT		889
#define RC5_TIME_LONG		1778
#define RC5_TIME_TOLERANCE	200

typedef enum {
	RC5_STATE_START1,
	RC5_STATE_MID1,
	RC5_STATE_START0,
	RC5_STATE_MID0
} RC5_State_t;

typedef union {
	struct {
		uint16_t command : 6;
		uint16_t address : 5;
		uint16_t toggle : 1;
		uint16_t start : 2;
	};
	uint16_t frame;
} RC5_Message_t;

typedef struct {
	uint8_t bits_ready;
	RC5_Message_t message;

	TIM_HandleTypeDef *htim;
	GPIO_TypeDef *rx_port;
	uint16_t rx_pin;
} RC5_FSM_Data_t;

void rc5_emit1(void *);
void rc5_emit0(void *);

uint8_t rc5_get_short_space(void *);
uint8_t rc5_get_short_pulse(void *);
uint8_t rc5_get_long_space(void *);
uint8_t rc5_get_long_pulse(void *);

#endif
