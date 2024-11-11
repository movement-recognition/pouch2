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

        uint16_t calib_dig_T1;
        int16_t calib_dig_T2, calib_dig_T3;
        uint16_t calib_dig_P1;
        int16_t calib_dig_P2, calib_dig_P3;
        int16_t calib_dig_P4, calib_dig_P5, calib_dig_P6;
        int16_t calib_dig_P7, calib_dig_P8, calib_dig_P9;
        uint8_t calib_dig_H1, calib_dig_H3;
        int16_t calib_dig_H2, calib_dig_H4, calib_dig_H5;
        int8_t calib_dig_H6;
        int32_t calib_t_fine;

        // compensation methods see original library from Bosch Sensortec:
        // https://github.com/boschsensortec/BME280_SensorAPI
        int32_t compensate_temperature(const int32_t raw_temperature);
        uint32_t compensate_pressure(const int32_t raw_pressure);
        uint32_t compensate_humidity(const uint32_t raw_humidity);
};      

#endif