#include <stdio.h>
#include "state_machine.h"

void execute_turn_on_state() {
    printf("Light is shines\n");
}

void execute_turn_off_state() {
    printf("Light is not shines\n");
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
    State_t on_state = {0, NULL, &execute_turn_on_state, NULL};
    State_t off_state = {1, NULL, &execute_turn_off_state, NULL};
    Event_t turn_on_event = {0, &get_turn_on_event};
    Event_t turn_off_event = {1, &get_turn_off_event};

    StateMachine_Init(&state_machine, off_state);

    StateMachine_AddState(&state_machine, on_state);
    StateMachine_AddState(&state_machine, off_state);
    StateMachine_AddEvent(&state_machine, turn_on_event);
    StateMachine_AddEvent(&state_machine, turn_off_event);

    StateMachine_AddTransition(&state_machine, on_state, off_state, turn_off_event);
    StateMachine_AddTransition(&state_machine, off_state, on_state, turn_on_event);

    do {

        scanf("%s[^\n]", input);

        StateMachine_Update(&state_machine);

    } while(input[0]!='x');

    StateMachine_Deinit(&state_machine);

    return 0;
}