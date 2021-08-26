#include "constants.h"
#include "fsm.h"


void fsm_switch_state(uint32_t millis_pressed, uint32_t current_time) {
	switch(fsm_current_state) {
		case STATE_IDLE:
			fsm_next_state = (millis_pressed >= 1000) ? STATE_LIGHTING : fsm_current_state;
			break;
		default:
			break;

	}
	time_last_state_change = (fsm_next_state != fsm_current_state) ? current_time : time_last_state_change;
	fsm_current_state = fsm_next_state;
}

void fsm_behavior(uint32_t current_time)
{
	uint32_t time_in_state = (current_time - time_last_state_change);
	switch(fsm_current_state) {
		case STATE_LIGHTING:
			dk_set_led(leds[MIN((int) (time_in_state/1000), 3)] , 1);
			break;
		default:
			break;
	}

}

void fsm_input_button(uint32_t button_state, uint32_t has_changed, uint32_t current_time)
{
	//printk("states : "BYTE_TO_BINARY_PATTERN" changed : "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(button_state), BYTE_TO_BINARY(has_changed));

	if (has_changed & USER_BUTTON) {
		if(!button_state) {
			if(time_last_pressed) {
				uint32_t time_released = current_time;
				uint32_t milliseconds_pressed = time_released - time_last_pressed;
				//printk("button has been released at %u , it was pressed for %u ms\n", time_released, milliseconds_pressed);

				fsm_switch_state(milliseconds_pressed, current_time);
			}
			time_last_pressed = 0;
		} else
		{
			time_last_pressed = current_time;
			//printk("button pressed at %u ms\n", time_last_pressed);
		}
	}
}