/**
 * @file fsm_event.h
 * @author Eryk Możdżeń
 * @date 2021-12-31
 */

#ifndef INC_FSM_EVENT_H_
#define INC_FSM_EVENT_H_

/**
 * @brief event object
 */
typedef struct {
	uint32_t priority;			/*< priority of event inside state, event is more important if has larger value */

	void (*transfer)(void *);   /*< function called when the event occurs and it has the highest priority, set NULL if not needed  */
	uint8_t (*get)(void *);     /*< function to check if event occurs, set NULL if no event is needed to switch state */

    uint32_t next_index;		/*< index of next state */
} __Event_t;

#endif
