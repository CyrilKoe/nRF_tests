#ifndef _FSM_H
#define _FSM_H

#include <zephyr/types.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
  (byte & 0x8 ? '1' : '0'), \
  (byte & 0x4 ? '1' : '0'), \
  (byte & 0x2 ? '1' : '0'), \
  (byte & 0x1 ? '1' : '0')

typedef enum
{
	STATE_IDLE, STATE_LIGHTING, STATE_LIGHTED, STATE_BLINKING, STATE_SHUTTING
} fsm_state_t;

fsm_state_t fsm_current_state, fsm_next_state;
uint32_t time_last_pressed, time_last_state_change; // overflow only after 2982 days


void fsm_behavior(uint32_t current_time);
void fsm_switch_state(uint32_t millis_pressed, uint32_t current_time);
void fsm_input_button(uint32_t button_state, uint32_t has_changed, uint32_t current_time);


#endif //_FSM_H