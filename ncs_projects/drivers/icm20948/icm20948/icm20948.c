/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT invensense_icm20948

#include <device.h>
#include <drivers/i2c.h>
#include <kernel.h>
#include <drivers/sensor.h>
#include <sys/__assert.h>
#include <logging/log.h>

#include "icm20948.h"

LOG_MODULE_REGISTER(ICM20948, CONFIG_SENSOR_LOG_LEVEL);

static uint8_t read8(const struct device *dev, uint8_t reg)
{
	uint8_t buf;
	if (i2c_reg_read_byte(dev, ICM20948_I2C_DEV_ID, reg, &buf) < 0) {
		LOG_ERR("Error reading register.");
	}
	return buf;
}

static int icm20948_sample_fetch(const struct device *dev,
			     enum sensor_channel chan)
{
	struct icm20948_data *drv_data = dev->data;
	__ASSERT_NO_MSG(chan == SENSOR_CHAN_ALL);
	drv_data->sample = read8(dev, ICM20948_REG_WHO_AM_I);
	return 0;
}

static int icm20948_channel_get(const struct device *dev,
			    enum sensor_channel chan,
			    struct sensor_value *val)
{
	return 0;
}

static const struct sensor_driver_api icm20948_driver_api = {
	.sample_fetch = icm20948_sample_fetch,
	.channel_get = icm20948_channel_get,
};

static int icm20948_init(const struct device *dev)
{
	struct icm20948_data *drv_data = dev->data;

	drv_data->i2c = device_get_binding(DT_INST_BUS_LABEL(0));
	if (drv_data->i2c == NULL) {
		LOG_ERR("Failed to get pointer to %s device!",
			    DT_INST_BUS_LABEL(0));
		return -EINVAL;
	}

	uint8_t chip_id;

	if(i2c_reg_read_byte(drv_data->i2c, ICM20948_I2C_DEV_ID, ICM20948_REG_WHO_AM_I, &chip_id) < 0)
	{
		LOG_ERR("Failed reading chip id");
	}

	LOG_INF("ICM20948 device ID = %x\n", chip_id);

	return 0;
}

struct icm20948_data icm20948_driver;

DEVICE_DT_INST_DEFINE(0, icm20948_init, NULL,
		    &icm20948_driver, NULL,
		    POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,
		    &icm20948_driver_api);
