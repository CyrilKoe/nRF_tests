- Copy the folder `icm20948` to the folder `/zephyr/drivers/sensor`
- Copy `invensense,icm20948.yaml` to the folder `zephyr/dts/bindings/sensor/`
- Add the following line to to `zephyr/driver/sensor/CMakeLists/txt`
```
add_subdirectory_ifdef(CONFIG_ICM20948 icm20948)
```

- Add the following line to `zephyr/driver/sensor/Kconfig`
```
source "drivers/sensor/icm20948/Kconfig"
```
- Copy the `boards` folder to your project root and modify the layout for your board (`i2c` pins)
- Add the following line to your `prj.conf` :
```
CONFIG_I2C=y
CONFIG_SENSOR=y
CONFIG_ICM20948=y
```

__On Linux__
```
ln -s $(pwd)/icm20948 $ZEPHYR_BASE/drivers/sensor/icm20948
ln -s $(pwd)/invensense,icm20948.yaml $ZEPHYR_BASE/dts/bindings/sensor/invensense,icm20948.yaml
```