#ifndef ENVIRONMENT_BME280_SENSOR_H
#define ENVIRONMENT_BME280_SENSOR_H

#include <vector>
#include "IEnvironmentalSensor.h"

class BME280_Sensor: public IEnvironmentalSensor {
    public:
        BME280_Sensor(II2C* ic2_bus, uint8_t i2c_addr=0x76);

        environment_struct get_environmental_data();
    private:
        II2C *i2c_bus;
        uint8_t i2c_addr;
        uint8_t calibration_data[42];
};

#endif