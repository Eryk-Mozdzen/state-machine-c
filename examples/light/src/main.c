/**
 * @file main.c
 * @author Eryk Możdżeń
 * @brief Simpele example how to use this state machine.
 * @date 2021-12-25
 */

#include <stdio.h>
#include "finite_state_machine.h"

// state and events identifiers
// it can be any non-negative number 
#define STATE_ON    0
#define STATE_OFF   1

#define EVENT_ON    0
#define EVENT_OFF   1

// execute functions for state definitions
// must returns void and takes void* argument
void execute_turn_on_state(void *input) {
    printf("Light is shining\n");
}

void execute_turn_off_state(void *input) {
    printf("Light is not shining\n");
}

// get functions for event definitions
// must returns uint8_t and takes void* argument
uint8_t get_turn_on_event(void *input) {
    return *(char *)input=='1';
}

uint8_t get_turn_off_event(void *input) {
    return *(char *)input=='2';
}

int main() {

    // place for user input
    char input = 'q';

    FiniteStateMachine_t state_machine;

    // states and events definitions
    State_t on_state        = {STATE_ON,    NULL,   &execute_turn_on_state,     NULL};
    State_t off_state       = {STATE_OFF,   NULL,   &execute_turn_off_state,    NULL};
    Event_t turn_on_event   = {EVENT_ON,    &get_turn_on_event  };
    Event_t turn_off_event  = {EVENT_OFF,   &get_turn_off_event };

    // init state machine, link user input
    FiniteStateMachine_Init(&state_machine, &input);

    // define states end events
    FiniteStateMachine_DefineState(&state_machine, on_state);
    FiniteStateMachine_DefineState(&state_machine, off_state);

    FiniteStateMachine_DefineEvent(&state_machine, turn_on_event);
    FiniteStateMachine_DefineEvent(&state_machine, turn_off_event);

    // define state transitions
    FiniteStateMachine_DefineTransition(&state_machine, STATE_ON, STATE_OFF, EVENT_OFF);  // if is in STATE_ON and EVENT_OFF occurres, change state to STATE_OFF
    FiniteStateMachine_DefineTransition(&state_machine, STATE_OFF, STATE_ON, EVENT_ON);   // if is in STATE_OFF and EVENT_ON occurres, change state to STATE_ON

    // begin from initial state
    FiniteStateMachine_Start(&state_machine, STATE_OFF);

    do {

        // read character from terminal
        // avoid endline character
        char tmp[8] = {0};
        scanf("%s[^\n]", tmp);
        input = tmp[0];

        // take care for all events, state transitions and state functions
        FiniteStateMachine_Update(&state_machine);

    } while(input!='x');

    // free memory
    FiniteStateMachine_Deinit(&state_machine);

    return 0;
}