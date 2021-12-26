/**
 * @file events.h
 * @author Eryk Możdzeń
 * @brief Set of events functions for light example
 * @date 2021-12-26
 */

#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <stdint.h>

typedef enum {
    EVENT_ON,
    EVENT_OFF
} LightEvent_t;

uint8_t get_turn_on_event(void *);
uint8_t get_turn_off_event(void *);

#endif