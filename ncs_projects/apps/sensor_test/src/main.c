#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif

void main(void)
{
	const struct device *dev, *dev2;
	bool led_is_on = false;
	int ret;
        const char *const label = DT_LABEL(DT_INST(0, invensense_icm20948));

	dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}
        printk("Led found\n");

  	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
		return;
	}
        printk("Led configured\n");

        dev2 = device_get_binding(label);
	if (dev2 == NULL) {
        	led_is_on = true;
	} else {
                printk("Sensor found\n");
        }

        gpio_pin_set(dev, PIN, (int)led_is_on);

        int rc;

	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
}
