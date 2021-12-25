/**
 * @file state_machine.h
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#define __STATE_MACHINE_UNKNOWN_STATE_INDEX   -1
#define __STATE_MACHINE_UNKNOWN_EVENT_INDEX   -1

typedef struct {
    uint32_t id;

    void (*enter)(void *);
    void (*execute)(void *);
    void (*exit)(void *);
} State_t;

typedef struct {
    uint32_t id;

    uint8_t (*get)(void *);
} Event_t;

typedef struct {
    int32_t curr_state_index;

    State_t *states;
    Event_t *events;
    int32_t **transitions;
    uint32_t states_num;
    uint32_t events_num;

    void *buffer;
} StateMachine_t;

void StateMachine_Init(StateMachine_t *, void *);
void StateMachine_Deinit(StateMachine_t *);

void StateMachine_DefineState(StateMachine_t *, State_t);
void StateMachine_DefineEvent(StateMachine_t *, Event_t);
void StateMachine_DefineTransition(StateMachine_t *, uint32_t, uint32_t, uint32_t);

void StateMachine_Start(StateMachine_t *, uint32_t);
void StateMachine_Update(StateMachine_t *);

#endif