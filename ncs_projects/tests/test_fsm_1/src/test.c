
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

/* Initialize test */
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

/* Finalize test */
int test_suiteTearDown(int num_failures)
{
	return generic_suiteTearDown(num_failures);
}

// Button is not pressed enough
void test_state_transition_1(void)
{
	time_last_pressed = 0;
	uint32_t current_time = 999;
	fsm_switch_state(true, current_time);
	TEST_ASSERT_EQUAL_INT(fsm_current_state, STATE_IDLE);
	TEST_ASSERT_EQUAL_INT(fsm_next_state, STATE_IDLE);
}


// Button is pressed enough
void test_state_transition_2(void)
{
	time_last_pressed = 0;
	uint32_t current_time = 1000;
	fsm_switch_state(true, current_time);
	TEST_ASSERT_EQUAL_INT(fsm_current_state, STATE_LIGHTING);
	TEST_ASSERT_EQUAL_INT(fsm_next_state, STATE_LIGHTING);
}


// Button is pressed very long
void test_state_transition_3(void)
{
	time_last_pressed = 0;
	uint32_t current_time = 9999999999999999999;
	fsm_switch_state(true, current_time);
	TEST_ASSERT_EQUAL_INT(fsm_current_state, STATE_LIGHTING);
	TEST_ASSERT_EQUAL_INT(fsm_next_state, STATE_LIGHTING);
}


// Button is released in the past
void test_state_transition_4(void)
{
	time_last_pressed = 2000;
	uint32_t current_time = 999;
	fsm_switch_state(true, current_time);
	TEST_ASSERT_EQUAL_INT(fsm_current_state, STATE_IDLE);
	TEST_ASSERT_EQUAL_INT(fsm_next_state, STATE_IDLE);
}

// Button release is negative
void test_state_transition_5(void)
{
	time_last_pressed = 2000;
	uint32_t current_time = -500;
	fsm_switch_state(true, current_time);
	TEST_ASSERT_EQUAL_INT(fsm_current_state, STATE_IDLE);
	TEST_ASSERT_EQUAL_INT(fsm_next_state, STATE_IDLE);
}

// Button pressed is negative
void test_state_transition_6(void)
{
	time_last_pressed = -400;
	uint32_t current_time = 1000;
	fsm_switch_state(true, current_time);
	TEST_ASSERT_EQUAL_INT(fsm_current_state, STATE_IDLE);
	TEST_ASSERT_EQUAL_INT(fsm_next_state, STATE_IDLE);
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