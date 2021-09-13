
#include <stdio.h>

#include <stdbool.h>
#include <unity.h>

#include "constants.h"
#include "fsm.h"

#include <mock_dk_buttons_and_leds.h>


// FSM states
extern fsm_state_t fsm_current_state, fsm_next_state;
extern bool state_change_request;

// Time events recordings
extern uint32_t time_last_pressed, time_last_state_change; // overflow only after 2982 days

// Callback functions
void callback_set_leds_state(uint32_t led_on_mask, uint32_t led_off_mask, int call_count);

void setUp(void)
{
	fsm_current_state = STATE_IDLE;
	fsm_next_state = STATE_IDLE;
	state_change_request = false;
	time_last_pressed = 0;
	time_last_state_change = 0;
}

/* Suite teardown shall finalize with mandatory call to generic_suiteTearDown. */
extern int generic_suiteTearDown(int num_failures);

/* Finalize test*/
int test_suiteTearDown(int num_failures)
{
	return generic_suiteTearDown(num_failures);
}


// Button is not pressed enough
void test_leds_output_1(void)
{
	__wrap_dk_set_leds_state_Stub(callback_set_leds_state);

	time_last_pressed = 0;
	uint32_t current_time = 1000;
	fsm_switch_state(true, current_time);

	TEST_ASSERT_EQUAL_INT(time_last_state_change, current_time);

	fsm_behavior(current_time);

	current_time += 500;

	fsm_behavior(current_time);
}

// Button is not pressed enough
void test_leds_output_2(void)
{
	__wrap_dk_set_leds_state_Stub(callback_set_leds_state);

	fsm_current_state = STATE_LIGHTING;
	fsm_next_state = STATE_LIGHTING;

	time_last_state_change = 1000;
	time_last_pressed = 1500;
	uint32_t current_time = 2000;

	fsm_switch_state(true, current_time);

	TEST_ASSERT_EQUAL_INT(fsm_next_state, STATE_LIGHTING);
	TEST_ASSERT_EQUAL_INT(fsm_current_state, STATE_LIGHTING);

	fsm_behavior(current_time);

	current_time += 500;

	fsm_behavior(current_time);
}


/* It is required to be added to each test. That is because unity is using
 * different main signature (returns int) and zephyr expects main which does
 * not return value.
 */
extern int unity_main(void);

void main(void)
{
	(void)unity_main();
}

void callback_set_leds_state(uint32_t led_on_mask, uint32_t led_off_mask, int call_count) {
	printf("on : %u  off : %u\n", led_on_mask, led_off_mask);
	switch(call_count) {
		case 0:
			TEST_ASSERT_EQUAL_INT(8+4, led_on_mask);
			TEST_ASSERT_EQUAL_INT(2+1, led_off_mask);
			break;
		case 1:
			TEST_ASSERT_EQUAL_INT(8+4+2, led_on_mask);
			TEST_ASSERT_EQUAL_INT(1, led_off_mask);
			break;
		default:
			break;
	}
}