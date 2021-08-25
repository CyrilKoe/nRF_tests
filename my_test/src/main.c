
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/printk.h>
#include <sys/byteorder.h>

#include <dk_buttons_and_leds.h>
#include <drivers/gpio.h>
#include <settings/settings.h>
#include <soc.h>
#include <zephyr/types.h>
#include <zephyr.h>

#define REFRESH_TIME            500

#define USER_BUTTON             DK_BTN4_MSK

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
  (byte & 0x8 ? '1' : '0'), \
  (byte & 0x4 ? '1' : '0'), \
  (byte & 0x2 ? '1' : '0'), \
  (byte & 0x1 ? '1' : '0')

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef enum
{
	STATE_IDLE, STATE_LIGHTING, STATE_LIGHTED, STATE_BLINKING, STATE_SHUTTING
} fsm_state_t;

static void button_changed(uint32_t button_state, uint32_t has_changed);
static int init_button(void);
void fsm_behavior();
static void fsm_switch_state(uint32_t millis_pressed);



/******** GLOBAL VARIABLES ********/

const uint8_t leds[4] = {DK_LED1, DK_LED2, DK_LED3, DK_LED4};
fsm_state_t fsm_current_state, fsm_next_state;
uint32_t time_last_pressed, time_last_state_change; // overflow only after 2982 days


/******** MAIN ********/


void main(void)
{
	int err;

	printk("Starting example\n");

	err = dk_leds_init();
	if (err) {
		printk("LEDs init failed (err %d)\n", err);
		return;
	}

	err = init_button();
	if (err) {
		printk("Button init failed (err %d)\n", err);
		return;
	}

	for (;;) {
		//printk("time : %u \n", k_uptime_get_32());
		fsm_behavior();

		k_sleep(K_MSEC(REFRESH_TIME));
	}
}


/******** FSM ********/


static void fsm_switch_state(uint32_t millis_pressed) {
	switch(fsm_current_state) {
		case STATE_IDLE:
			fsm_next_state = (millis_pressed > 1000) ? STATE_LIGHTING : fsm_current_state;
			break;
		default:
			break;

	}
	time_last_state_change = (fsm_next_state != fsm_current_state) ? k_uptime_get_32() : time_last_state_change;
	fsm_current_state = fsm_next_state;
}

void fsm_behavior()
{
	uint32_t time_in_state = (k_uptime_get_32() - time_last_state_change);
	switch(fsm_current_state) {
		case STATE_LIGHTING:
			dk_set_led(leds[MAX((int) (time_in_state/1000), 3)] , 1);
			break;
		default:
			break;
	}

}


/******** BUTTONS ********/


static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	printk("states : "BYTE_TO_BINARY_PATTERN" changed : "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(button_state), BYTE_TO_BINARY(has_changed));

	if (has_changed & USER_BUTTON) {
		if(!button_state) {
			if(time_last_pressed) {
				uint32_t time_released = k_uptime_get_32();
				uint32_t milliseconds_pressed = time_released - time_last_pressed;
				printk("button has been released at %u , it was pressed for %u ms\n", time_released, milliseconds_pressed);

				fsm_switch_state(milliseconds_pressed);
			}
			time_last_pressed = 0;
		} else
		{
			time_last_pressed = k_uptime_get_32();
			printk("button pressed at %u ms\n", time_last_pressed);
		}
	}
}

static int init_button(void)
{
	int err;

	err = dk_buttons_init(button_changed);
	if (err) {
		printk("Cannot init buttons (err: %d)\n", err);
	}

	return err;
}