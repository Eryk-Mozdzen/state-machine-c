/**
 * @file finite_state_machine.h
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef __FINITE_STATE_MACHINE_H__
#define __FINITE_STATE_MACHINE_H__

/**
 * @brief event definition data
 */
typedef struct {
    uint32_t priority;

    void (*execute)(void *);    /*< function called when the event occurs and it has the highest priority, set NULL if not needed  */
    uint8_t (*get)(void *);     /*< function to check if event occurs, must be defined */
} EventConfig_t;

/**
 * @brief state definition data
 */
typedef struct {
    uint32_t id;                /*< user identifier of specific state, must be uniq */
    
    void (*enter)(void *);      /*< function called once, at the beginning of the state, after switch from previous one, set NULL if not needed */
    void (*execute)(void *);    /*< function called every time when the state is used, set NULL if not needed  */
    void (*exit)(void *);       /*< function called once, at the end of the state, before switch to the next one, set NULL if not needed  */
} StateConfig_t;

typedef struct {
    EventConfig_t config;		/*< event configuration data */

    uint32_t next_index;		/*< index of next state */
} Event_t;

typedef struct {
	StateConfig_t config;		/*< state configuration data */

	Event_t *events;			/*< set of defined events for this state */
    uint32_t events_num;        /*< number of defined events for this state */
} State_t;

/**
 * @brief state machine object structure
 */
typedef struct {
    uint32_t curr_state_index;  /*< index of current state */

    State_t *states;            /*< indexed set of defined states */
    uint32_t states_num;        /*< number of defined states */

    void *buffer;               /*< memory buffer for user data, allocated and deallocated by user, is passed ass argument to all state and event functions */
} FiniteStateMachine_t;

void FiniteStateMachine_Init(FiniteStateMachine_t *, void *);
void FiniteStateMachine_Deinit(FiniteStateMachine_t *);

uint8_t FiniteStateMachine_DefineState(FiniteStateMachine_t *, StateConfig_t);
uint8_t FiniteStateMachine_DefineTransition(FiniteStateMachine_t *, uint32_t, uint32_t, EventConfig_t);

uint8_t FiniteStateMachine_Start(FiniteStateMachine_t *, uint32_t);
uint8_t FiniteStateMachine_Update(FiniteStateMachine_t *);

#endif
