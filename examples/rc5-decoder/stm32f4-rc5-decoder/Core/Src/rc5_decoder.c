/*
 * rc5_decoder.c
 *
 *  Created on: Dec 30, 2021
 *      Author: ermoz
 */

#include "rc5_decoder.h"

void rc5_emit1(void *data) {
	uint8_t bits = ((RC5_FSM_Data_t *)data)->bits_ready;
	RC5_Message_t msg = ((RC5_FSM_Data_t *)data)->message;

	msg.frame |=(1<<(14 - bits));

	((RC5_FSM_Data_t *)data)->message = msg;
	((RC5_FSM_Data_t *)data)->bits_ready++;
}

void rc5_emit0(void *data) {
	((RC5_FSM_Data_t *)data)->bits_ready++;
}
