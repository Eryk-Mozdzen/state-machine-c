/**
 * @file state_machine.c
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#include "state_machine.h"

static int32_t __StateMachine_GetStateIndex(StateMachine_t *, State_t);
static int32_t __StateMachine_GetEventIndex(StateMachine_t *, Event_t);

/**
 * @brief Initialize a new StateMachine object
 * @param st pointer to used state machine
 * @param initial initial state
 */
void StateMachine_Init(StateMachine_t *st, State_t initial) {
    // init fields
    st->curr_state_index = __STATE_MACHINE_UNKNOWN_STATE_INDEX;
    st->states_num = 0;
    st->events_num = 0;
    st->states = NULL;
    st->events = NULL;
    st->transitions = NULL;

    // add initial state to database
    StateMachine_DefineState(st, initial);
    
    // set current state
    st->curr_state_index = __StateMachine_GetStateIndex(st, initial);
    if(initial.enter!=NULL)
        initial.enter();
}

void StateMachine_Deinit(StateMachine_t *st) {
    // deallocate all memory
    for(uint32_t i=0; i<st->states_num; i++)
        free(st->transitions[i]);
    free(st->transitions);
    free(st->states);
    free(st->events);
    
    // reset fields
    st->curr_state_index = __STATE_MACHINE_UNKNOWN_STATE_INDEX;
    st->states_num = 0;
    st->events_num = 0;
    st->transitions = NULL;
    st->states = NULL;
    st->events = NULL;
}

void StateMachine_DefineState(StateMachine_t *st, State_t state) {
    // check if already exist
    if(__StateMachine_GetStateIndex(st, state)!=__STATE_MACHINE_UNKNOWN_STATE_INDEX)
        return;

    // allocate memory
    st->states_num++;
    st->states = (State_t *)realloc(st->states, st->states_num*sizeof(State_t));
    st->transitions = (int32_t **)realloc(st->transitions, st->states_num*sizeof(int32_t *));

    // write and init transitions
    st->states[st->states_num-1] = state;
    for(uint32_t j=0; j<st->events_num; j++)
        st->transitions[st->states_num-1][j] = st->states_num-1;
}

void StateMachine_DefineEvent(StateMachine_t *st, Event_t event) {
    assert(event.get!=NULL);

    // check if already exist
    if(__StateMachine_GetEventIndex(st, event)!=__STATE_MACHINE_UNKNOWN_EVENT_INDEX)
        return;

    // allocate memory
    st->events_num++;
    st->events = (Event_t *)realloc(st->events, st->states_num*sizeof(Event_t));
    for(uint32_t i=0; i<st->states_num; i++)
        st->transitions[i] = (int32_t *)realloc(st->transitions[i], st->events_num*sizeof(int32_t));

    // write and init transitions
    st->events[st->events_num-1] = event;
    for(uint32_t i=0; i<st->states_num; i++)
        st->transitions[i][st->events_num-1] = i;
}

void StateMachine_DefineTransition(StateMachine_t *st, State_t prev, State_t next, Event_t event) {
    assert(__StateMachine_GetStateIndex(st, prev)!=__STATE_MACHINE_UNKNOWN_STATE_INDEX);
    assert(__StateMachine_GetStateIndex(st, next)!=__STATE_MACHINE_UNKNOWN_STATE_INDEX);
    assert(__StateMachine_GetEventIndex(st, event)!=__STATE_MACHINE_UNKNOWN_EVENT_INDEX);

    // get internal indecies
    const int32_t prev_index = __StateMachine_GetStateIndex(st, prev);
    const int32_t next_index = __StateMachine_GetStateIndex(st, next);
    const int32_t event_index = __StateMachine_GetEventIndex(st, event);

    // set transition
    st->transitions[prev_index][event_index] = next_index;
}

void StateMachine_Update(StateMachine_t *st) {

    // check if any event occurs
    // @todo assume that is only one event per iteration
    int32_t event_index = __STATE_MACHINE_UNKNOWN_EVENT_INDEX;
    for(uint32_t i=0; i<st->events_num; i++) {
        if(st->events[i].get()) {
            event_index = i;
            break;
        }
    }

    if(event_index!=__STATE_MACHINE_UNKNOWN_EVENT_INDEX) {
        // get next state index
        int32_t next_state_index = st->transitions[st->curr_state_index][event_index];

        // check if next state is diffrent than current one
        if(next_state_index!=st->curr_state_index) {
            // call exit function for current state if exist
            if(st->states[st->curr_state_index].exit!=NULL)
                st->states[st->curr_state_index].exit();

            // call enter function for next state if exist
            if(st->states[next_state_index].enter!=NULL)
                st->states[next_state_index].enter();
        }

        // change current state
        st->curr_state_index = next_state_index;
    }
    
    // call execute function for current state if exist
    if(st->states[st->curr_state_index].execute!=NULL)
        st->states[st->curr_state_index].execute();
}

/**
 * @brief Search for internal index of specific state.
 * @param st pointer to state machine
 * @param state state
 * @return internal index of this state, unknown if state does not exist
 */
int32_t __StateMachine_GetStateIndex(StateMachine_t *st, State_t state) {
    for(uint32_t i=0; i<st->states_num; i++) {
        if(st->states[i].id==state.id)
            return i;
    }
    
    return __STATE_MACHINE_UNKNOWN_STATE_INDEX;
}

/**
 * @brief Search for internal index of specific event.
 * @param st pointer to state machine
 * @param event event
 * @return internal index of this event, unknown if state does not exist
 */
int32_t __StateMachine_GetEventIndex(StateMachine_t *st, Event_t event) {
    for(uint32_t i=0; i<st->events_num; i++) {
        if(st->events[i].id==event.id)
            return i;
    }
    
    return __STATE_MACHINE_UNKNOWN_EVENT_INDEX;
}