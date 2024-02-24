#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include "fsm/fsm.h"

static struct fsm_state * find_state(fsm_t *fsm, uint8_t id) {
    for(uint8_t i=0; i<fsm->states_num; i++) {
        if(fsm->states[i].id==id) {
			return &fsm->states[i];
        }
    }

    return NULL;
}

void fsm_add_state(fsm_t *fsm, uint8_t id, fsm_callback_t enter, fsm_callback_t execute, fsm_callback_t exit) {
    assert(fsm->states_num<FSM_STATE_MAX_NUM);

    fsm->states[fsm->states_num].id = id;
    fsm->states[fsm->states_num].enter = enter;
    fsm->states[fsm->states_num].execute = execute;
    fsm->states[fsm->states_num].exit = exit;
	fsm->states[fsm->states_num].events_num = 0;
	fsm->states_num++;
}

void fsm_add_transition(fsm_t *fsm, uint8_t from, uint8_t to, fsm_trigger_t trigger, fsm_callback_t action) {
    struct fsm_state *from_state = find_state(fsm, from);
	struct fsm_state *to_state = find_state(fsm, to);

    assert(from_state);
    assert(to_state);
    assert(from_state->events_num<FSM_EVENT_MAX_NUM);

    from_state->events[from_state->events_num].trigger = trigger;
    from_state->events[from_state->events_num].action = action;
    from_state->events[from_state->events_num].next = to_state;
	from_state->events_num++;
}

void fsm_start(fsm_t *fsm, uint8_t initial) {
    struct fsm_state *initial_state = find_state(fsm, initial);

    assert(initial_state);

	fsm->current = initial_state;

    if(fsm->current->enter) {
        fsm->current->enter(fsm->context);
    }
}

void fsm_update(fsm_t *fsm) {
    assert(fsm->current);

	struct fsm_event *event = NULL;

    for(uint8_t i=0; i<fsm->current->events_num; i++) {
    	if(fsm->current->events[i].trigger) {
    		if(fsm->current->events[i].trigger(fsm->context)) {
                event = &fsm->current->events[i];
                break;
            }
		} else {
			event = &fsm->current->events[i];
            break;
		}
    }

	if(event) {
        if(fsm->current->exit) {
            fsm->current->exit(fsm->context);
        }

        if(event->action) {
            event->action(fsm->context);
        }

        fsm->current = event->next;

        if(fsm->current->enter) {
            fsm->current->enter(fsm->context);
        }
    }
}

void fsm_execute(fsm_t *fsm) {
    assert(fsm->current);

	if(fsm->current->execute) {
        fsm->current->execute(fsm->context);
    }
}
