/**
 * @file states.h
 * @author Eryk Możdżeń
 * @brief Set of state functions for light example
 * @date 2021-12-26
 */

#ifndef __STATES_H__
#define __STATES_H__

#include "data.h"
#include <stdio.h>

typedef enum {
    STATE_ON,
    STATE_OFF
} LightState_t;

void enter_turn_on_state(void *);
void execute_turn_on_state(void *);
void exit_turn_on_state(void *);

void enter_turn_off_state(void *);
void execute_turn_off_state(void *);
void exit_turn_off_state(void *);

#endif