#ifndef FSM_FSM_H
#define FSM_FSM_H

#include <stdbool.h>
#include <stdint.h>

#include "fsm/config.h"

typedef void (*fsm_callback_t)(void *);
typedef bool (*fsm_trigger_t)(const void *);

struct fsm_state;

struct fsm_event {
	fsm_trigger_t trigger;
    fsm_callback_t action;
    struct fsm_state *next;
};

struct fsm_state {
	uint8_t id;
	fsm_callback_t enter;
	fsm_callback_t execute;
	fsm_callback_t exit;

	struct fsm_event events[FSM_EVENT_MAX_NUM];
	uint8_t events_num;
};

typedef struct {
    void *context;

	struct fsm_state *current;
	struct fsm_state states[FSM_STATE_MAX_NUM];
	uint8_t states_num;
} fsm_t;

void fsm_add_state(fsm_t *fsm, uint8_t id, fsm_callback_t enter, fsm_callback_t execute, fsm_callback_t exit);
void fsm_add_transition(fsm_t *fsm, uint8_t from, uint8_t to, fsm_trigger_t trigger, fsm_callback_t action);

void fsm_start(fsm_t *fsm, uint8_t initial);
void fsm_update(fsm_t *fsm);
void fsm_execute(fsm_t *fsm);

#endif
