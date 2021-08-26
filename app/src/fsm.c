#include "constants.h"
#include "fsm.h"

#if defined(CONFIG_ARCH_POSIX)
#define printk printf
#else
#include <dk_buttons_and_leds.h>
#include <sys/printk.h>
#endif

// FSM states
fsm_state_t fsm_current_state, fsm_next_state;
bool state_change_request;
// Time events recordings
uint32_t time_last_pressed, time_last_state_change; // overflow only after 2982 days

// Leds addresses and masks
const uint8_t leds[4] = {LED_1, LED_2, LED_3, LED_4};
const uint8_t leds_msk[4] = {LED_1_MSK, LED_2_MSK, LED_3_MSK, LED_4_MSK};

// This function is only called in case of event (state_change_request turned to true, or button pressed)

/* Function : fsm_switch_state
 * Computes the next fsm state
 * this function is only called in case of event (state_change_request turned to true, or button pressed)
 */
void fsm_switch_state(uint32_t current_time) {

	uint32_t millis_pressed = current_time - time_last_pressed;
	uint32_t millis_in_state = current_time - time_last_state_change;

	switch(fsm_current_state) {
		case STATE_IDLE:
			fsm_next_state = (millis_pressed >= 1000) ? STATE_LIGHTING : fsm_current_state;
			break;
		case STATE_LIGHTING:
			fsm_next_state = state_change_request ? STATE_LIGHTED : fsm_current_state;
			break;
		case STATE_LIGHTED:
			fsm_next_state = state_change_request ? STATE_BLINKING : fsm_current_state;
			break;
		case STATE_BLINKING:
			fsm_next_state = (millis_pressed >= 2000) ? STATE_SHUTTING : fsm_current_state;
			break;
		case STATE_SHUTTING:
			fsm_next_state = state_change_request ? STATE_IDLE : fsm_current_state;
			break;
		default:
			break;

	}

	// Record the moment of the state change
	time_last_state_change = (fsm_next_state != fsm_current_state) ? current_time : time_last_state_change;

	fsm_current_state = fsm_next_state;
	state_change_request = false;
}

/* Function : fsm_behaviors
 * Computes the outptus of the current state
 * this function is called periodically by the infinite loop
 */
void fsm_behavior(uint32_t current_time)
{
	uint32_t millis_in_state = current_time - time_last_state_change;
	printk("State %u since %u\n", fsm_current_state, millis_in_state);

	uint32_t on_mask = 0, off_mask = 0;

	switch(fsm_current_state) {
		case STATE_LIGHTING:
			// Build masks with the firsts leds off
			for(unsigned int i = 0; i < 4; i++) {
				if(((int) (millis_in_state/500)) > i) {
					on_mask |= leds_msk[3-i];
				}
				else
					off_mask |= leds_msk[3-i];
			}
			// Apply changes
			dk_set_leds_state(on_mask, off_mask);
			// Set change signal if all leds are on
			if(!off_mask && on_mask)
				state_change_request = true;
			break;

		case STATE_LIGHTED:
			if(millis_in_state >= 2000) {
				dk_set_leds_state(0, leds_msk[0]|leds_msk[1]|leds_msk[2]|leds_msk[3]);
				state_change_request = true;
			}
			break;
	
		case STATE_BLINKING:
			dk_set_led(leds[0], (millis_in_state % 1000) > 500);
			break;
	
		case STATE_SHUTTING:
			// Build masks with the firsts leds on
			for(unsigned int i = 0; i < 4; i++) {
				if(((int) (millis_in_state/500)) > i) {
					off_mask |= leds_msk[i];
				}
				else
					on_mask |= leds_msk[i];
			}
			// Apply changes
			dk_set_leds_state(on_mask, off_mask);
			// Set change signal if all leds are off
			if(!on_mask && off_mask)
				state_change_request = true;
			break;

		default:
			break;
	
	}

	if (state_change_request)
		fsm_switch_state(current_time);

}


/* Function : fsm_input_button
 * Computes the time between the button has been pressed and released
 * this function is called when a button is pressed
 */
void fsm_input_button(uint32_t button_state, uint32_t has_changed, uint32_t current_time)
{
	printk("Buttons states : "BYTE_TO_BINARY_PATTERN" changed : "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(button_state), BYTE_TO_BINARY(has_changed));

	if (has_changed & USER_BUTTON) {
		if(!button_state) {
			if(time_last_pressed) {
				uint32_t time_released = current_time;
				uint32_t milliseconds_pressed = time_released - time_last_pressed;
				printk("button has been released at %u , it was pressed for %u ms\n", time_released, milliseconds_pressed);

				fsm_switch_state(current_time);
			}
			time_last_pressed = 0;
		} else
		{
			time_last_pressed = current_time;
			printk("button pressed at %u ms\n", time_last_pressed);
		}
	}
}