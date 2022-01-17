/**
 * @file fsm_state.h
 * @author Eryk Możdżeń
 * @date 2021-12-31
 */

#ifndef INC_FSM_STATE_H_
#define INC_FSM_STATE_H_

#include <stdint.h>
#include "fsm_event.h"

/**
 * @brief state object
 */
typedef struct {
	uint32_t id;                /*< user identifier of specific state, must be uniq */

	void (*enter)(void *);      /*< function called once, at the beginning of the state, after switch from previous one, set NULL if not needed */
	void (*execute)(void *);    /*< function called every time when the state is used, set NULL if not needed  */
	void (*exit)(void *);       /*< function called once, at the end of the state, before switch to the next one, set NULL if not needed  */

	__Event_t *events;			/*< set of defined events for this state */
    uint32_t events_num;        /*< number of defined events for this state */
} __State_t;

#endif
