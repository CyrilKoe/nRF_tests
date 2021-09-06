#ifndef FSM_H__
#define FSM_H__

#include <zephyr/types.h>

// Pretty print macros
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
  (byte & 0x8 ? '1' : '0'), \
  (byte & 0x4 ? '1' : '0'), \
  (byte & 0x2 ? '1' : '0'), \
  (byte & 0x1 ? '1' : '0')

// States of the FSM
typedef enum
{
	STATE_IDLE, STATE_LIGHTING, STATE_LIGHTED, STATE_BLINKING, STATE_SHUTTING
} fsm_state_t;

// FSM functions
void fsm_behavior(uint32_t current_time);
void fsm_switch_state(uint8_t has_button_event, uint32_t current_time);
void fsm_input_button(uint32_t button_state, uint32_t has_changed, uint32_t current_time);


#endif //FSM_H__