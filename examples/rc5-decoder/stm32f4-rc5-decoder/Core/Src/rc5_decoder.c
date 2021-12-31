/*
 * rc5_decoder.c
 *
 *  Created on: Dec 30, 2021
 *      Author: ermoz
 */

#include "rc5_decoder.h"

void rc5_emit1(void *data) {
	if(((RC5_FSM_Data_t *)data)->bits_ready==14)
		return;

	((RC5_FSM_Data_t *)data)->message.frame |=(1<<(13 - ((RC5_FSM_Data_t *)data)->bits_ready));
	((RC5_FSM_Data_t *)data)->bits_ready++;
}

void rc5_emit0(void *data) {
	if(((RC5_FSM_Data_t *)data)->bits_ready==14)
		return;

	((RC5_FSM_Data_t *)data)->message.frame &=~(1<<(13 - ((RC5_FSM_Data_t *)data)->bits_ready));
	((RC5_FSM_Data_t *)data)->bits_ready++;
}

uint8_t rc5_get_short_space(void *data) {
	if(HAL_GPIO_ReadPin(((RC5_FSM_Data_t *)data)->rx_port, ((RC5_FSM_Data_t *)data)->rx_pin)==GPIO_PIN_SET) {
		uint32_t time = __HAL_TIM_GET_COUNTER(((RC5_FSM_Data_t *)data)->htim);

		if(abs(time - RC5_TIME_SHORT)<RC5_TIME_TOLERANCE) {
			__HAL_TIM_SET_COUNTER(((RC5_FSM_Data_t *)data)->htim, 0);
			return 1;
		}

	}

	return 0;
}

uint8_t rc5_get_short_pulse(void *data) {
	if(HAL_GPIO_ReadPin(((RC5_FSM_Data_t *)data)->rx_port, ((RC5_FSM_Data_t *)data)->rx_pin)==GPIO_PIN_RESET) {
		uint32_t time = __HAL_TIM_GET_COUNTER(((RC5_FSM_Data_t *)data)->htim);

		if(abs(time - RC5_TIME_SHORT)<RC5_TIME_TOLERANCE) {
			__HAL_TIM_SET_COUNTER(((RC5_FSM_Data_t *)data)->htim, 0);
			return 1;
		}
	}

	return 0;
}

uint8_t rc5_get_long_space(void *data) {
	if(HAL_GPIO_ReadPin(((RC5_FSM_Data_t *)data)->rx_port, ((RC5_FSM_Data_t *)data)->rx_pin)==GPIO_PIN_SET) {
		uint32_t time = __HAL_TIM_GET_COUNTER(((RC5_FSM_Data_t *)data)->htim);

		if(abs(time - RC5_TIME_LONG)<RC5_TIME_TOLERANCE) {
			__HAL_TIM_SET_COUNTER(((RC5_FSM_Data_t *)data)->htim, 0);
			return 1;
		}
	}

	return 0;
}

uint8_t rc5_get_long_pulse(void *data) {
	if(HAL_GPIO_ReadPin(((RC5_FSM_Data_t *)data)->rx_port, ((RC5_FSM_Data_t *)data)->rx_pin)==GPIO_PIN_RESET) {
		uint32_t time = __HAL_TIM_GET_COUNTER(((RC5_FSM_Data_t *)data)->htim);

		if(abs(time - RC5_TIME_LONG)<RC5_TIME_TOLERANCE) {
			__HAL_TIM_SET_COUNTER(((RC5_FSM_Data_t *)data)->htim, 0);
			return 1;
		}
	}

	return 0;
}

