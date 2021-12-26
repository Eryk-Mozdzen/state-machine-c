/**
 * @file main.c
 * @author Eryk Możdżeń
 * @brief Example how to use this state machine.
 * @date 2021-12-26
 */

#include "state_machine.h"
#include "states.h"
#include "events.h"
#include "non_block.h"  // kbhit, getch
#include <unistd.h>     // usleep

int main() {

    // place for user input
    char input = 'q';
    set_conio_terminal_mode();

    StateMachine_t state_machine;

    // init state machine, link user input
    StateMachine_Init(&state_machine, &input);

    // define states end events
    StateMachine_DefineState(&state_machine, (State_t){STATE_ON,    &enter_turn_on_state,   &execute_turn_on_state,     &exit_turn_on_state});
    StateMachine_DefineState(&state_machine, (State_t){STATE_OFF,   &enter_turn_off_state,  &execute_turn_off_state,    &exit_turn_off_state});

    StateMachine_DefineEvent(&state_machine, (Event_t){EVENT_ON,    &get_turn_on_event  });
    StateMachine_DefineEvent(&state_machine, (Event_t){EVENT_OFF,   &get_turn_off_event });

    // define state transitions
    StateMachine_DefineTransition(&state_machine, STATE_ON, STATE_OFF, EVENT_OFF);  // if is in STATE_ON and EVENT_OFF occurres, change state to STATE_OFF
    StateMachine_DefineTransition(&state_machine, STATE_OFF, STATE_ON, EVENT_ON);   // if is in STATE_OFF and EVENT_ON occurres, change state to STATE_ON

    // begin from initial state
    StateMachine_Start(&state_machine, STATE_OFF);

    while(input!='x') {
        if(kbhit())
            input = getch();

        StateMachine_Update(&state_machine);

        usleep(200000);
    }

    // free memory
    StateMachine_Deinit(&state_machine);
    
    reset_terminal_mode();

    return 0;
}