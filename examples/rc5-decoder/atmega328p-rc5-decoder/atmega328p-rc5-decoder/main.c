#define F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "finite_state_machine.h"

typedef enum {
	RC5_STATE_START1,
	RC5_STATE_MID1,
	RC5_STATE_START0,
	RC5_STATE_MID0
} RC5_State_t;

typedef enum {
	RC5_EVENT_LONG_SPACE,
	RC5_EVENT_SHORT_SPACE,
	RC5_EVENT_LONG_PULSE,
	RC5_EVENT_SHORT_PULSE
} RC5_Event_t;

typedef union {
	struct {
		uint8_t toggle : 1;
		uint8_t address : 5;
		uint8_t command : 6;
	};
	
	uint16_t raw_data;
} RC5_Message_t;

typedef struct {
	uint8_t bits_ready;
	RC5_Message_t message;
} RC5_FSM_Data_t;

void rc5_emit1(void *data) {
	uint8_t bits = ((RC5_FSM_Data_t *)data)->bits_ready;
	RC5_Message_t msg = ((RC5_FSM_Data_t *)data)->message;
	
	msg.raw_data |=(1<<(14 - bits));
	
	((RC5_FSM_Data_t *)data)->message = msg;
	((RC5_FSM_Data_t *)data)->bits_ready++;
}

void rc5_emit0(void *data) {
	((RC5_FSM_Data_t *)data)->bits_ready++;
}

FiniteStateMachine_t fsm;

ISR(PCINT2_vect) {
	FiniteStateMachine_Update(&fsm);
}

int main() {
	
	DDRD &= ~_BV(DDD2);
	PCMSK2 |= _BV(PCINT18);
	PCICR |= _BV(PCIE2);
	
	RC5_FSM_Data_t data;
	data.bits_ready = 0;
	data.message.raw_data = 0;
	
	FiniteStateMachine_Init(&fsm, &data);
	
	FiniteStateMachine_DefineState(&fsm, (State_t){RC5_STATE_START1,	NULL,		NULL, NULL});
	FiniteStateMachine_DefineState(&fsm, (State_t){RC5_STATE_MID1,		&rc5_emit1, NULL, NULL});
	FiniteStateMachine_DefineState(&fsm, (State_t){RC5_STATE_START0,	NULL,		NULL, NULL});
	FiniteStateMachine_DefineState(&fsm, (State_t){RC5_STATE_MID0,		&rc5_emit0, NULL, NULL});

	FiniteStateMachine_DefineEvent(&fsm, (Event_t){RC5_EVENT_LONG_SPACE,	NULL});
	FiniteStateMachine_DefineEvent(&fsm, (Event_t){RC5_EVENT_SHORT_SPACE,	NULL});
	FiniteStateMachine_DefineEvent(&fsm, (Event_t){RC5_EVENT_LONG_PULSE,	NULL});
	FiniteStateMachine_DefineEvent(&fsm, (Event_t){RC5_EVENT_SHORT_PULSE,	NULL});
		
	FiniteStateMachine_DefineTransition(&fsm, RC5_STATE_START1, RC5_STATE_MID1, RC5_EVENT_SHORT_SPACE);
	FiniteStateMachine_DefineTransition(&fsm, RC5_STATE_MID1, RC5_STATE_START1, RC5_EVENT_SHORT_PULSE);
	FiniteStateMachine_DefineTransition(&fsm, RC5_STATE_MID1, RC5_STATE_MID0, RC5_EVENT_LONG_PULSE);
	FiniteStateMachine_DefineTransition(&fsm, RC5_STATE_MID0, RC5_STATE_MID1, RC5_EVENT_LONG_SPACE);
	FiniteStateMachine_DefineTransition(&fsm, RC5_STATE_START0, RC5_STATE_MID0, RC5_EVENT_SHORT_SPACE);
	FiniteStateMachine_DefineTransition(&fsm, RC5_STATE_MID0, RC5_STATE_START0, RC5_EVENT_SHORT_PULSE);
	
	FiniteStateMachine_Start(&fsm, RC5_STATE_MID1);
	
	sei();
    
    while(1) {
		if(data.bits_ready<14)
			continue;
			
		data.bits_ready = 0;
		data.message.raw_data = 0;
			
		printf("Toggle: %u\tAddress: 0x%02X\tCommand: 0x%02X\n", data.message.toggle, data.message.address, data.message.command);
		
    }
}

