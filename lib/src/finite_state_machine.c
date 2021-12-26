/**
 * @file finite_state_machine.c
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#include "finite_state_machine.h"

/**
 * @brief functions only for internal use
 */
static uint8_t __FiniteStateMachine_GetStateIndex(FiniteStateMachine_t *, uint32_t, uint32_t *);
static uint8_t __FiniteStateMachine_GetEventIndex(FiniteStateMachine_t *, uint32_t, uint32_t *);

/**
 * @brief Initialize fields in state machine structure
 * @param st pointer to state machine
 * @param buffer memory buffer for user data
 */
void FiniteStateMachine_Init(FiniteStateMachine_t *st, void *buffer) {
    // init fields
    st->curr_state_index = 0;
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
void FiniteStateMachine_Deinit(FiniteStateMachine_t *st) {
    // deallocate all memory
    for(uint32_t i=0; i<st->states_num; i++)
        free(st->transitions[i]);
    free(st->transitions);
    free(st->states);
    free(st->events);
    
    // reset fields
    st->curr_state_index = 0;
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
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t FiniteStateMachine_DefineState(FiniteStateMachine_t *st, State_t state) {
    // check if already exist
    if(!__FiniteStateMachine_GetStateIndex(st, state.id, NULL))
        return EXIT_FAILURE;

    // allocate memory
    st->states_num++;

    st->states = (State_t *)realloc(st->states, st->states_num*sizeof(State_t));
    if(!st->states)
        return EXIT_FAILURE;

    st->transitions = (uint32_t **)realloc(st->transitions, st->states_num*sizeof(uint32_t *));
    if(!st->transitions)
        return EXIT_FAILURE;

    // write and init transitions
    st->states[st->states_num-1] = state;
    for(uint32_t j=0; j<st->events_num; j++)
        st->transitions[st->states_num-1][j] = st->states_num-1;
    
    return EXIT_SUCCESS;
}

/**
 * @brief Add event definition to state machine
 * @param st pointer to state machine
 * @param event event definition
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t FiniteStateMachine_DefineEvent(FiniteStateMachine_t *st, Event_t event) {
    // check if already exist
    if(!__FiniteStateMachine_GetEventIndex(st, event.id, NULL))
        return EXIT_FAILURE;

    // allocate memory
    st->events_num++;

    st->events = (Event_t *)realloc(st->events, st->states_num*sizeof(Event_t));
    if(!st->events)
        return EXIT_FAILURE;

    for(uint32_t i=0; i<st->states_num; i++) {
        st->transitions[i] = (uint32_t *)realloc(st->transitions[i], st->events_num*sizeof(uint32_t));
        if(!st->transitions[i])
            return EXIT_FAILURE;
    }

    // write and init transitions
    st->events[st->events_num-1] = event;
    for(uint32_t i=0; i<st->states_num; i++)
        st->transitions[i][st->events_num-1] = st->events_num-1;

    return EXIT_SUCCESS;
}

/**
 * @brief Set transition between two states caused by presents of specific event
 * @param st pointer to state machine
 * @param curr_id identifier of the state when event occurred
 * @param next_id identifier of the state that should be set after event occurred
 * @param event_id identifier of event causing transition
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t FiniteStateMachine_DefineTransition(FiniteStateMachine_t *st, uint32_t curr_id, uint32_t next_id, uint32_t event_id) {
    uint32_t curr_index = 0;
    uint32_t next_index = 0;
    uint32_t event_index = 0;

    // get internal indexes
    if(__FiniteStateMachine_GetStateIndex(st, curr_id, &curr_index))
        return EXIT_FAILURE;
    if(__FiniteStateMachine_GetStateIndex(st, next_id, &next_index))
        return EXIT_FAILURE;
    if(__FiniteStateMachine_GetEventIndex(st, event_id, &event_index))
        return EXIT_FAILURE;

    // set transition
    st->transitions[curr_index][event_index] = next_index;

    return EXIT_SUCCESS;
}

/**
 * @brief Set inital state for state machine, call enter function if exist
 * @param st pointer to state machine
 * @param initial_id identifier of state that should be at the begin of the program, must be defined before
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t FiniteStateMachine_Start(FiniteStateMachine_t *st, uint32_t initial_id) {
    if(__FiniteStateMachine_GetStateIndex(st, initial_id, &st->curr_state_index))
        return EXIT_FAILURE;

    // call enter function for initial state
    if(st->states[st->curr_state_index].enter!=NULL)
        st->states[st->curr_state_index].enter(st->buffer);

    return EXIT_SUCCESS;
}

/**
 * @brief Check if any event occurred, change state if it's required and call exit, enter, execute functions
 * @param st pointer to state machine
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t FiniteStateMachine_Update(FiniteStateMachine_t *st) {
    if(!st->states_num)
        return EXIT_FAILURE;

    // check if any event occurs
    // @todo assume that is only one event per iteration
    uint8_t event_flag = 0;
    uint32_t event_index = 0;
    for(uint32_t i=0; i<st->events_num; i++) {
        if(st->events[i].get(st->buffer)) {
            event_index = i;
            event_flag = 1;
            break;
        }
    }

    if(event_flag) {
        // get next state index
        const uint32_t next_state_index = st->transitions[st->curr_state_index][event_index];

        // check if next state is diffrent than current one
        if(next_state_index!=st->curr_state_index) {
            // call exit function for current state if exist
            if(st->states[st->curr_state_index].exit!=NULL)
                st->states[st->curr_state_index].exit(st->buffer);

            // call enter function for next state if exist
            if(st->states[next_state_index].enter!=NULL)
                st->states[next_state_index].enter(st->buffer);
                
            // change current state
            st->curr_state_index = next_state_index;
        }
    }
    
    // call execute function for current state if exist
    if(st->states[st->curr_state_index].execute!=NULL)
        st->states[st->curr_state_index].execute(st->buffer);

    return EXIT_SUCCESS;
}

/**
 * @brief Search for internal index of specific state.
 * @param st pointer to state machine
 * @param state_id identifier of the state
 * @param state_index internal index of this state, unknown if state does not exist
 * @return status of operation, 0 if success, 1 if state not defined
 */
uint8_t __FiniteStateMachine_GetStateIndex(FiniteStateMachine_t *st, uint32_t state_id, uint32_t *state_index) {
    for(uint32_t i=0; i<st->states_num; i++) {
        if(st->states[i].id==state_id) {
            if(state_index!=NULL)
                *state_index = i;
            return EXIT_SUCCESS;
        }
    }
    
    return EXIT_FAILURE;
}

/**
 * @brief Search for internal index of specific event.
 * @param st pointer to state machine
 * @param event_id identifier of the event
 * @param event_index internal index of this event, unknown if event does not exist
 * @return status of operation, 0 if success, 1 if event not defined
 */
uint8_t __FiniteStateMachine_GetEventIndex(FiniteStateMachine_t *st, uint32_t event_id, uint32_t *event_index) {
    for(uint32_t i=0; i<st->events_num; i++) {
        if(st->events[i].id==event_id) {
            if(event_index!=NULL)
                *event_index = i;
            return EXIT_SUCCESS;
        }
    }
    
    return EXIT_FAILURE;
}