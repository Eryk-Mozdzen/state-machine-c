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
static void __FiniteStateMachine_CallStateEnter(FiniteStateMachine_t *, uint32_t);
static void __FiniteStateMachine_CallStateExecute(FiniteStateMachine_t *, uint32_t);
static void __FiniteStateMachine_CallStateExit(FiniteStateMachine_t *, uint32_t);

/**
 * @brief Initialize fields in state machine structure
 * @param st pointer to state machine
 * @param buffer memory buffer for user data
 */
void FiniteStateMachine_Init(FiniteStateMachine_t *st, void *buffer) {
    // init fields
    st->curr_state_index = 0;
    st->states_num = 0;
    st->states = NULL;
    st->buffer = buffer;
}

/**
 * @brief Free memory allocated by state machine
 * @param st pointer to state machine
 */
void FiniteStateMachine_Deinit(FiniteStateMachine_t *st) {
    // deallocate all memory
    for(uint32_t i=0; i<st->states_num; i++)
        free(st->states[i].events);
    free(st->states);
    
    // reset fields
    st->curr_state_index = 0;
    st->states_num = 0;
    st->states = NULL;
}

/**
 * @brief Add state definition to state machine
 * @param st pointer to state machine
 * @param state_config state definition
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t FiniteStateMachine_DefineState(FiniteStateMachine_t *st, StateConfig_t state_config) {
    // check if already exist
    if(!__FiniteStateMachine_GetStateIndex(st, state_config.id, NULL))
        return EXIT_FAILURE;

    // allocate memory
    st->states_num++;
    st->states = (State_t *)realloc(st->states, st->states_num*sizeof(State_t));
    if(!st->states)
        return EXIT_FAILURE;

    // write init values
    st->states[st->states_num-1].config = state_config;
    st->states[st->states_num-1].events = NULL;
    st->states[st->states_num-1].events_num = 0;
    
    return EXIT_SUCCESS;
}

/**
 * @brief Set transition between two states caused by presents of specific event
 * @param st pointer to state machine
 * @param curr_id identifier of the state when event occurred
 * @param next_id identifier of the state that should be set after event occurred
 * @param event_config definition of event causing transition
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t FiniteStateMachine_DefineTransition(FiniteStateMachine_t *st, uint32_t curr_id, uint32_t next_id, EventConfig_t event_config) {
    uint32_t curr_index = 0;
    uint32_t next_index = 0;

    // get internal indexes
    if(__FiniteStateMachine_GetStateIndex(st, curr_id, &curr_index))
        return EXIT_FAILURE;
    if(__FiniteStateMachine_GetStateIndex(st, next_id, &next_index))
        return EXIT_FAILURE;

    // allocate memory
    st->states[curr_index].events_num++;
    st->states[curr_index].events = (Event_t *)realloc(st->states[curr_index].events, st->states[curr_index].events_num*sizeof(Event_t));
    if(!st->states[curr_index].events)
    	return EXIT_FAILURE;

    // write data
    st->states[curr_index].events[st->states[curr_index].events_num-1].config = event_config;
    st->states[curr_index].events[st->states[curr_index].events_num-1].next_index = next_index;

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
    if(st->states[st->curr_state_index].config.enter!=NULL)
        st->states[st->curr_state_index].config.enter(st->buffer);

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

    // check if any event defined for current state occurs
    Event_t *event = NULL;
    for(uint32_t i=0; i<st->states[st->curr_state_index].events_num; i++) {
    	// check if event occurs
		if(st->states[st->curr_state_index].events[i].config.get(st->buffer)) {
			// if more than one event compare priorities
			if(event) {
				if(st->states[st->curr_state_index].events[i].config.priority<event->config.priority)
					continue;
			}

			event = &st->states[st->curr_state_index].events[i];
		}
    }

    if(!event) {
		// call exit function for current state if exist
		if(st->states[st->curr_state_index].config.exit!=NULL)
			st->states[st->curr_state_index].config.exit(st->buffer);

		// call enter function for next state if exist
		if(st->states[event->next_index].config.enter!=NULL)
			st->states[event->next_index].config.enter(st->buffer);

		// change current state
		st->curr_state_index = event->next_index;
    }
    
    // call execute function for current state if exist
    if(st->states[st->curr_state_index].config.execute!=NULL)
        st->states[st->curr_state_index].config.execute(st->buffer);

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
        if(st->states[i].config.id==state_id) {
            if(state_index!=NULL)
                *state_index = i;
            return EXIT_SUCCESS;
        }
    }
    
    return EXIT_FAILURE;
}


