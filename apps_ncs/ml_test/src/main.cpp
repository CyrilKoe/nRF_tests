#include <dk_buttons_and_leds.h>
#include <zephyr.h>

#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "edge-impulse-sdk/dsp/numpy.hpp"


#define USER_BUTTON 4

// Hard code the data to classify
static const float features[] = {
    -1.7300, 1.4700, 11.6400, -1.6400, 1.3600, 12.5200, -1.8300, 1.7100, 12.9000, -1.8200, 1.9900, 12.7400, -1.9800, 2.8400, 12.4700, -1.9800, 2.8400, 12.4700, -2.3000, 3.0700, 13.1300, -2.2500, 2.6800, 14.7900, -2.1500, 2.4900, 15.9400, -2.3400, 2.5100, 15.8100, -2.7600, 3.0400, 15.5600, -2.7200, 3.3000, 15.9600, -2.7200, 3.3000, 15.9600, -2.2800, 2.6000, 16.6800, -2.2900, 2.1000, 16.0600, -2.5700, 2.3500, 14.5200, -2.5100, 2.9600, 13.7200, -2.3500, 2.9700, 13.4000, -2.1800, 3.4400, 12.6500, -2.0700, 3.3800, 11.7900, -2.0700, 3.3800, 11.7900, -1.3800, 3.1600, 12.4900, -1.0200, 3.1600, 12.6100, -0.9500, 3.1900, 11.7200, -1.0600, 3.0100, 10.6900, -0.8800, 2.9000, 10.2600, -0.4400, 2.0600, 10.4000, -0.4400, 2.0600, 10.4000, -0.3600, 1.1100, 10.4700, -0.1300, 1.0500, 10.9100, -0.2100, 1.6200, 10.2300, -0.0800, 1.8800, 9.1100, -0.0500, 1.2900, 8.3200, 0.1900, 1.0800, 7.9100, 0.1900, 1.0800, 7.9100, 0.4600, 1.0000, 7.9400, 0.8600, 0.8600, 8.3300, 0.9100, 0.7600, 7.9300, 0.9500, 0.6500, 7.0100, 0.9800, 0.1500, 5.8600, 1.0700, -0.1400, 5.7600, 1.3600, -0.5300, 5.7700, 1.3600, -0.5300, 5.7700, 1.4000, -0.7300, 5.2500, 1.2100, -1.0400, 5.0400, 1.1200, -0.8800, 4.9600, 1.4400, -0.1100, 4.3700, 1.3400, 0.5600, 3.6700, 1.3400, 0.9900, 3.7800, 1.3400, 0.9900, 3.7800, 1.4800, 1.0100, 4.5300, 1.3600, 0.8000, 4.8400, 0.8700, 0.8100, 4.8100, 0.5300, 0.4600, 5.3300, 0.7200, 0.8200, 6.1100, 1.1100, 1.6000, 6.7000, 1.1100, 1.6000, 6.7000, 1.1600, 1.9200, 6.6800, 0.6900, 1.6300, 6.8100, 0.1700, 1.1400, 7.7200, 0.4600, 1.1200, 9.0200, 0.4100, 1.4000, 9.2500, 0.0700, 2.3400, 8.9300, -0.2200, 2.7100, 9.3900, -0.2200, 2.7100, 9.3900, -0.0800, 2.2800, 10.4100, -0.0800, 1.9200, 10.7600, -0.3300, 2.0000, 10.4700, -0.8100, 2.0000, 10.4000, -1.2500, 3.0500, 10.5000, -0.9400, 4.2900, 11.3600, -0.9400, 4.2900, 11.3600, -0.6300, 4.3800, 12.5500, -0.5600, 4.0800, 12.2100, -0.9100, 3.1000, 11.2800, -1.6300, 1.4200, 11.3400, -1.9800, 1.1200, 12.5100, -1.2800, 2.1800, 13.9600, -1.2800, 2.1800, 13.9600, -1.5300, 3.1200, 14.6500, -2.5200, 3.2100, 15.0200, -2.6500, 3.6200, 16.2500, -2.5000, 3.8700, 17.7800, -2.1300, 3.7500, 18.8700, -2.2000, 3.3600, 18.7800, -2.4500, 3.0200, 17.4500, -2.4500, 3.0200, 17.4500, -2.5700, 2.0500, 16.1100, -2.6300, 1.6800, 15.0500, -2.5600, 2.2000, 14.1500, -2.0900, 2.8100, 13.7800, -1.9400, 2.1700, 13.0100, -1.9700, 1.5800, 11.8200, -1.9700, 1.5800, 11.8200, -1.3100, 2.1600, 11.3800, -1.3500, 2.2500, 10.8000, -0.9300, 1.5200, 10.2700, -0.7100, 0.9300, 10.0500, -0.4300, 1.0000, 9.9400, -0.2500, 1.0700, 9.7400, -0.2500, 1.0700, 9.7400, 0.1600, 1.0500, 9.6100, 0.0600, 0.9500, 9.2700, -0.2400, 0.9600, 8.8300, 0.2700, 0.9200, 8.6100, 0.6400, 0.5200, 8.3700, 0.9300, 0.5000, 8.1800, 1.1100, 0.4400, 8.5900, 1.1100, 0.4400, 8.5900, 0.9600, 0.1500, 8.4500, 0.5900, -0.2700, 7.3500, 0.5200, -0.6000, 6.3000, 0.9300, -0.3300, 5.7700, 1.1300, 0.4400, 5.1800, 1.0000, 0.4500, 5.1200, 1.0000, 0.4500, 5.1200, 0.9500, 0.1600, 4.9700, 1.0700, 0.2400, 4.5800, 0.9200, 0.0600, 4.1700, 0.5800, 0.2600, 3.9700, 0.3500, 0.6600, 4.2300, 0.5500, 0.8100, 5.3000, 0.5500, 0.8100, 5.3000, 0.5500, 0.4500, 5.7900, 0.5000, -0.0500, 5.7200
};


// Get a part of the flashed data
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

// Classification function
void run_classification() {
    ei_impulse_result_t result = { 0 };

    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = sizeof(features) / sizeof(features[0]);
    features_signal.get_data = &raw_feature_get_data;

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, true);
    printk("run_classifier returned: %d\n", res);

    printk("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);

    // print the predictions
    printk("[");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf_float(result.classification[ix].value);
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        printk(", ");
#else
        if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
            printk(", ");
        }
#endif
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf_float(result.anomaly);
#endif
    printk("]\n");
}

// Button callback
static void button_changed(uint32_t button_state, uint32_t has_changed) {
	if (has_changed & USER_BUTTON) {
        if(!button_state) {
            printk("Button pressed\n");
            run_classification();
        }
    }
}

// Button init
static int init_button(void)
{
	int err;

	err = dk_buttons_init(button_changed);
	if (err) {
		printk("Cannot init buttons (err: %d)\n", err);
	}

	return err;
}

// Main function
void main() {
    // This is needed so that output of printf is output immediately without buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    printk("Edge Impulse standalone inferencing (Zephyr)\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        printk("The size of your 'features' array is not correct. Expected %d items, but had %u\n",
            EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        return;
    }

    int err = init_button();
	if (err) {
		printk("Button init failed (err %d)\n", err);
		return;
	}

    while (1) {
        k_msleep(1000);
    }
}
