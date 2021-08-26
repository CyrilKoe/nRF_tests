
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/printk.h>
#include <sys/byteorder.h>

#include <dk_buttons_and_leds.h>
#include <drivers/gpio.h>
#include <settings/settings.h>
#include <soc.h>
#include <zephyr.h>

#include "constants.h"
#include "fsm.h"


static int init_button(void);


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
		fsm_behavior(k_uptime_get_32());

		k_sleep(K_MSEC(REFRESH_TIME));
	}
}

static void button_changed(uint32_t button_state, uint32_t has_changed) {
	fsm_input_button(button_state, has_changed, k_uptime_get_32());
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