#include <stdio.h>

#include "fsm/fsm.h"

typedef enum {
    EXAMPLE_STATE_ON,
    EXAMPLE_STATE_OFF
} example_state_t;

void execute_turn_on_state(void *context) {
    (void)context;

    printf("Light is shining\n");
}

void execute_turn_off_state(void *context) {
    (void)context;

    printf("Light is not shining\n");
}

bool trigger_turn_on_event(const void *context) {
    const char *input = (char *)context;

    return *input=='1';
}

bool trigger_turn_off_event(const void *context) {
    const char *input = (char *)context;

    return *input=='2';
}

int main() {

    char input = 'q';

    fsm_t fsm = {
        .context = &input
    };

    fsm_add_state(&fsm, EXAMPLE_STATE_ON,    NULL, execute_turn_on_state,    NULL);
    fsm_add_state(&fsm, EXAMPLE_STATE_OFF,   NULL, execute_turn_off_state,   NULL);

    fsm_add_transition(&fsm, EXAMPLE_STATE_ON,   EXAMPLE_STATE_OFF,  trigger_turn_off_event, NULL);
    fsm_add_transition(&fsm, EXAMPLE_STATE_OFF,  EXAMPLE_STATE_ON,   trigger_turn_on_event,  NULL);

    fsm_start(&fsm, EXAMPLE_STATE_ON);

    do {
        char tmp[8] = {0};
        scanf("%s[^\n]", tmp);
        input = tmp[0];

        fsm_update(&fsm);
        fsm_execute(&fsm);
    } while(input!='x');

    return 0;
}
