/**
 * @file states.c
 * @author Eryk Możdżeń
 * @brief Set of state functions for light example
 * @date 2021-12-26
 */

#include "states.h"

// enter, execute and exit functions for state definitions
// must returns void and takes void* argument

void enter_turn_on_state(void *data) {
    printf("Enter on state\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}

void execute_turn_on_state(void *data) {
    printf("Light is shining\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}

void exit_turn_on_state(void *data) {
    printf("Exit on state\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}

void enter_turn_off_state(void *data) {
    printf("Enter off state\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}

void execute_turn_off_state(void *data) {
    printf("Light is not shining\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}

void exit_turn_off_state(void *data) {
    printf("Exit off state\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}