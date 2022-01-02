/*
 * rc5_decoder.c
 *
 *  Created on: Dec 30, 2021
 *      Author: ermoz
 */

#include "rc5_decoder.h"
#include <stdio.h>

void rc5_emit1(void *data) {
	((RC5_FSM_Data_t *)data)->message.frame |=(1<<(13 - ((RC5_FSM_Data_t *)data)->bits_ready));
	((RC5_FSM_Data_t *)data)->bits_ready++;
}

void rc5_emit0(void *data) {
	((RC5_FSM_Data_t *)data)->message.frame &=~(1<<(13 - ((RC5_FSM_Data_t *)data)->bits_ready));
	((RC5_FSM_Data_t *)data)->bits_ready++;
}

uint8_t rc5_get_short_space(void *data) {
	if(((RC5_FSM_Data_t *)data)->state==GPIO_PIN_RESET) {
		uint32_t time = ((RC5_FSM_Data_t *)data)->counter*RC5_TIME_PRESCALER;

		if(abs(time - RC5_TIME_SHORT)<=RC5_TIME_TOLERANCE)
			return 1;
	}

	return 0;
}

uint8_t rc5_get_short_pulse(void *data) {
	if(((RC5_FSM_Data_t *)data)->state==GPIO_PIN_SET) {
		uint32_t time = ((RC5_FSM_Data_t *)data)->counter*RC5_TIME_PRESCALER;

		if(abs(time - RC5_TIME_SHORT)<=RC5_TIME_TOLERANCE)
			return 1;
	}

	return 0;
}

uint8_t rc5_get_long_space(void *data) {
	if(((RC5_FSM_Data_t *)data)->state==GPIO_PIN_RESET) {
		uint32_t time = ((RC5_FSM_Data_t *)data)->counter*RC5_TIME_PRESCALER;

		if(abs(time - RC5_TIME_LONG)<=RC5_TIME_TOLERANCE)
			return 1;
	}

	return 0;
}

uint8_t rc5_get_long_pulse(void *data) {
	if(((RC5_FSM_Data_t *)data)->state==GPIO_PIN_SET) {
		uint32_t time = ((RC5_FSM_Data_t *)data)->counter*RC5_TIME_PRESCALER;

		if(abs(time - RC5_TIME_LONG)<=RC5_TIME_TOLERANCE)
			return 1;
	}

	return 0;
}

