/**
 * @file main.c
 * @author Eryk Możdżeń
 * @brief Simpele example how to use this state machine.
 * @date 2021-12-25
 */

#include <stdio.h>
#include "state_machine.h"

#define STATE_ON    0
#define STATE_OFF   1

#define EVENT_ON    0
#define EVENT_OFF   1

void enter_turn_on_state() {
    printf("Turn on light\n");
}

void execute_turn_on_state() {
    printf("Light is shining\n");
}

void enter_turn_off_state() {
    printf("Turn off light\n");
}

void execute_turn_off_state() {
    printf("Light is not shining\n");
}

char input[8];

uint8_t get_turn_on_event() {
    return input[0]=='1';
}

uint8_t get_turn_off_event() {
    return input[0]=='2';
}

int main() {

    StateMachine_t state_machine;
    State_t on_state        = {STATE_ON,    &enter_turn_on_state,   &execute_turn_on_state,     NULL};
    State_t off_state       = {STATE_OFF,   &enter_turn_off_state,  &execute_turn_off_state,    NULL};
    Event_t turn_on_event   = {EVENT_ON,    &get_turn_on_event  };
    Event_t turn_off_event  = {EVENT_OFF,   &get_turn_off_event };

    // init state machine
    StateMachine_Init(&state_machine);

    // define states end events
    StateMachine_DefineState(&state_machine, on_state);
    StateMachine_DefineState(&state_machine, off_state);

    StateMachine_DefineEvent(&state_machine, turn_on_event);
    StateMachine_DefineEvent(&state_machine, turn_off_event);

    // define state transitions
    StateMachine_DefineTransition(&state_machine, STATE_ON, STATE_OFF, EVENT_OFF);  // if is in STATE_ON and EVENT_OFF occurres, change state to STATE_OFF
    StateMachine_DefineTransition(&state_machine, STATE_OFF, STATE_ON, EVENT_ON);   // if is in STATE_OFF and EVENT_ON occurres, change state to STATE_ON

    // begin from initial state
    StateMachine_Start(&state_machine, STATE_OFF);

    do {

        scanf("%s[^\n]", input);

        StateMachine_Update(&state_machine);

    } while(input[0]!='x');

    // free memory
    StateMachine_Deinit(&state_machine);

    return 0;
}