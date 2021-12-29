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
 * @brief state definition data
 */
typedef struct {
    uint32_t id;                /*< user identifier of specific state, must be uniq */
    
    void (*enter)(void *);      /*< function called once, at the beginning of the state, after switch from previous one, set NULL if not needed */
    void (*execute)(void *);    /*< function called every time when the state is used, set NULL if not needed  */
    void (*exit)(void *);       /*< function called once, at the end of the state, before switch to the next one, set NULL if not needed  */
} State_t;

/**
 * @brief event definition data
 */
typedef struct {
    uint32_t id;                /*< user identifier of specific event, must be uniq */

    uint8_t (*get)(void *);     /*< function to check if event occurs, must be defined */
} Event_t;

/**
 * @brief state machine object structure
 */
typedef struct {
    uint32_t curr_state_index;  /*< index, current state of the machine */

    State_t *states;            /*< indexed set of defined states */
    Event_t *events;            /*< indexed set of defined events */
    uint32_t **transitions;     /*< transition table, rows represents states, columns represents events */
    uint32_t states_num;        /*< number of defined states */
    uint32_t events_num;        /*< number of defined events */

    void *buffer;               /*< memory buffer for user data, allocated and deallocated by user, is passed ass argument to all state and event functions */
} FiniteStateMachine_t;

void FiniteStateMachine_Init(FiniteStateMachine_t *, void *);
void FiniteStateMachine_Deinit(FiniteStateMachine_t *);

uint8_t FiniteStateMachine_DefineState(FiniteStateMachine_t *, State_t);
uint8_t FiniteStateMachine_DefineEvent(FiniteStateMachine_t *, Event_t);
uint8_t FiniteStateMachine_DefineTransition(FiniteStateMachine_t *, uint32_t, uint32_t, uint32_t);

uint8_t FiniteStateMachine_Start(FiniteStateMachine_t *, uint32_t);
uint8_t FiniteStateMachine_Update(FiniteStateMachine_t *);

#endif