#ifndef ZEPHYR_DRIVERS_SENSOR_ICM20948_ICM20948_H_
#define ZEPHYR_DRIVERS_SENSOR_ICM20948_ICM20948_H_

#include <device.h>
#include <sys/util.h>

#define ICM20948_REG_WHO_AM_I           0x00
#define ICM20948_I2C_DEV_ID         0x68

struct icm20948_data {
	const struct device *i2c;
	uint8_t sample;
};

#endif /* _SENSOR_ICM20948_ */