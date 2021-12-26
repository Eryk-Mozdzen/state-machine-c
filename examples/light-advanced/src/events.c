/**
 * @file events.c
 * @author Eryk Możdzeń
 * @brief Set of events functions for light example
 * @date 2021-12-26
 */

#include "events.h"

// get functions for event definitions
// must returns uint8_t and takes void* argument
uint8_t get_turn_on_event(void *data) {
    return ((CustomUserData_t *)data)->input=='1';
}

uint8_t get_turn_off_event(void *data) {
    return ((CustomUserData_t *)data)->input=='2';
}