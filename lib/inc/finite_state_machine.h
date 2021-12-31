/**
 * @file finite_state_machine.h
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#ifndef __FINITE_STATE_MACHINE_H__
#define __FINITE_STATE_MACHINE_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "fsm_event.h"
#include "fsm_state.h"

/**
 * @brief state machine object structure
 */
typedef struct {
    uint32_t curr_state_index;  /*< index of current state */

    __State_t *states;			/*< indexed set of defined states */
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
