/**
 * @file state_machine.c
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#include "state_machine.h"

/**
 * @brief functions only for internal use
 */
static int32_t __StateMachine_GetStateIndex(StateMachine_t *, uint32_t);
static int32_t __StateMachine_GetEventIndex(StateMachine_t *, uint32_t);

/**
 * @brief Initialize fields in state machine structure
 * @param st pointer to state machine
 * @param buffer memory buffer for user data
 */
void StateMachine_Init(StateMachine_t *st, void *buffer) {
    // init fields
    st->curr_state_index = __STATE_MACHINE_UNKNOWN_STATE_INDEX;
    st->states_num = 0;
    st->events_num = 0;
    st->states = NULL;
    st->events = NULL;
    st->transitions = NULL;
    st->buffer = buffer;
}

/**
 * @brief Free memory allocated by state machine
 * @param st pointer to state machine
 */
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

/**
 * @brief Add state definition to state machine
 * @param st pointer to state machine
 * @param state state definition
 */
void StateMachine_DefineState(StateMachine_t *st, State_t state) {
    // check if already exist
    if(__StateMachine_GetStateIndex(st, state.id)!=__STATE_MACHINE_UNKNOWN_STATE_INDEX)
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

/**
 * @brief Add event definition to state machine
 * @param st pointer to state machine
 * @param event event definition
 */
void StateMachine_DefineEvent(StateMachine_t *st, Event_t event) {
    assert(event.get!=NULL);

    // check if already exist
    if(__StateMachine_GetEventIndex(st, event.id)!=__STATE_MACHINE_UNKNOWN_EVENT_INDEX)
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

/**
 * @brief Set transition between two states caused by presents of specific event
 * @param st pointer to state machine
 * @param curr_id identifier of the state when event occurred
 * @param next_id identifier of the state that should be set after event occurred
 * @param event_id identifier of event causing transition
 */
void StateMachine_DefineTransition(StateMachine_t *st, uint32_t curr_id, uint32_t next_id, uint32_t event_id) {
    assert(__StateMachine_GetStateIndex(st, curr_id)!=__STATE_MACHINE_UNKNOWN_STATE_INDEX);
    assert(__StateMachine_GetStateIndex(st, next_id)!=__STATE_MACHINE_UNKNOWN_STATE_INDEX);
    assert(__StateMachine_GetEventIndex(st, event_id)!=__STATE_MACHINE_UNKNOWN_EVENT_INDEX);

    // get internal indexes
    const int32_t curr_index = __StateMachine_GetStateIndex(st, curr_id);
    const int32_t next_index = __StateMachine_GetStateIndex(st, next_id);
    const int32_t event_index = __StateMachine_GetEventIndex(st, event_id);

    // set transition
    st->transitions[curr_index][event_index] = next_index;
}

/**
 * @brief Set inital state for state machine, call enter function if exist
 * @param st pointer to state machine
 * @param initial_id identifier of state that should be at the begin of the program, must be defined before
 */
void StateMachine_Start(StateMachine_t *st, uint32_t initial_id) {
    assert(__StateMachine_GetStateIndex(st, initial_id)!=__STATE_MACHINE_UNKNOWN_STATE_INDEX);

    st->curr_state_index = __StateMachine_GetStateIndex(st, initial_id);

    // call enter function for initial state
    if(st->states[st->curr_state_index].enter!=NULL)
        st->states[st->curr_state_index].enter(st->buffer);
}

/**
 * @brief Check if any event occurred, change state if it's required and call exit, enter, execute functions
 * @param st pointer to state machine
 */
void StateMachine_Update(StateMachine_t *st) {
    // check if any event occurs
    // @todo assume that is only one event per iteration
    int32_t event_index = __STATE_MACHINE_UNKNOWN_EVENT_INDEX;
    for(uint32_t i=0; i<st->events_num; i++) {
        if(st->events[i].get(st->buffer)) {
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
                st->states[st->curr_state_index].exit(st->buffer);

            // call enter function for next state if exist
            if(st->states[next_state_index].enter!=NULL)
                st->states[next_state_index].enter(st->buffer);
        }

        // change current state
        st->curr_state_index = next_state_index;
    }
    
    // call execute function for current state if exist
    if(st->states[st->curr_state_index].execute!=NULL)
        st->states[st->curr_state_index].execute(st->buffer);
}

/**
 * @brief Search for internal index of specific state.
 * @param st pointer to state machine
 * @param state_id identifier of the state
 * @return internal index of this state, unknown if state does not exist
 */
int32_t __StateMachine_GetStateIndex(StateMachine_t *st, uint32_t state_id) {
    for(uint32_t i=0; i<st->states_num; i++) {
        if(st->states[i].id==state_id)
            return i;
    }
    
    return __STATE_MACHINE_UNKNOWN_STATE_INDEX;
}

/**
 * @brief Search for internal index of specific event.
 * @param st pointer to state machine
 * @param event_id identifier of the event
 * @return internal index of this event, unknown if state does not exist
 */
int32_t __StateMachine_GetEventIndex(StateMachine_t *st, uint32_t event_id) {
    for(uint32_t i=0; i<st->events_num; i++) {
        if(st->events[i].id==event_id)
            return i;
    }
    
    return __STATE_MACHINE_UNKNOWN_EVENT_INDEX;
}