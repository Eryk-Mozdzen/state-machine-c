/**
 * @file main.c
 * @author Eryk Możdżeń
 * @brief Example how to use this state machine.
 * @date 2021-12-26
 */

#include "finite_state_machine.h"

#include "data.h"
#include "states.h"
#include "events.h"

#include "non_block.h"  // kbhit, getch
#include <unistd.h>     // usleep

int main() {
    
    set_conio_terminal_mode();

    // user data
    CustomUserData_t data = {'q'};

    FiniteStateMachine_t state_machine;

    // init state machine, link user input
    FiniteStateMachine_Init(&state_machine, &data);

    // define states end events
    FiniteStateMachine_DefineState(&state_machine, (State_t){STATE_ON,    &enter_turn_on_state,   &execute_turn_on_state,     &exit_turn_on_state});
    FiniteStateMachine_DefineState(&state_machine, (State_t){STATE_OFF,   &enter_turn_off_state,  &execute_turn_off_state,    &exit_turn_off_state});

    FiniteStateMachine_DefineEvent(&state_machine, (Event_t){EVENT_ON,    &get_turn_on_event  });
    FiniteStateMachine_DefineEvent(&state_machine, (Event_t){EVENT_OFF,   &get_turn_off_event });

    // define state transitions
    FiniteStateMachine_DefineTransition(&state_machine, STATE_ON, STATE_OFF, EVENT_OFF);  // if is in STATE_ON and EVENT_OFF occurres, change state to STATE_OFF
    FiniteStateMachine_DefineTransition(&state_machine, STATE_OFF, STATE_ON, EVENT_ON);   // if is in STATE_OFF and EVENT_ON occurres, change state to STATE_ON

    // begin from initial state
    FiniteStateMachine_Start(&state_machine, STATE_OFF);

    // loop unit user click 'x' on the keyboard
    while(data.input!='x') {
        if(kbhit())
            data.input = getch();

        FiniteStateMachine_Update(&state_machine);

        usleep(200000);
    }

    // free memory
    FiniteStateMachine_Deinit(&state_machine);
    
    reset_terminal_mode();

    return 0;
}