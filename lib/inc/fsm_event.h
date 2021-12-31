/**
 * @file fsm_event.h
 * @author Eryk Możdżeń
 * @date 2021-12-31
 */

#ifndef INC_FSM_EVENT_H_
#define INC_FSM_EVENT_H_

/**
 * @brief event definition data
 */
typedef struct {
    uint32_t priority;

    void (*execute)(void *);    /*< function called when the event occurs and it has the highest priority, set NULL if not needed  */
    uint8_t (*get)(void *);     /*< function to check if event occurs, must be defined */
} EventConfig_t;

/**
 * @brief event object
 */
typedef struct {
    EventConfig_t config;		/*< event configuration data */

    uint32_t next_index;		/*< index of next state */
} __Event_t;

#endif
