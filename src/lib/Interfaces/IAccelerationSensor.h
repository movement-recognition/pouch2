#ifndef ILINESENSOR_H
#define ILINESENSOR_H

#include "II2C.h"

struct acceleration_struct {
    // fixed point: all values 1000* (in m*s^-2 or deg*s^-1 or Kelvin)
    int64_t timestamp = 0;
    int32_t accel_x = 0;
    int32_t accel_y = 0;
    int32_t accel_z = 0;
    int32_t gyro_x = 0;
    int32_t gyro_y = 0;
    int32_t gyro_z = 0;
    int32_t temperature = 0;
};

class IAccelerationSensor {
    public:
        IAccelerationSensor() {};
        virtual ~IAccelerationSensor() {};

        virtual acceleration_struct get_imu_data() = 0;

};

#endif