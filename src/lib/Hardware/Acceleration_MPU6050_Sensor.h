#ifndef ACCELERATION_MPU6050_SENSOR_H
#define ACCELERATION_MPU6050_SENSOR_H

#include "IAccelerationSensor.h"

struct mpu6050_acceleration_struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    int16_t temperature;
};

enum mpu6050_gyro_range {
    gyro_range_250 = 0,
    gyro_range_500 = 1,
    gyro_range_1000 = 2,
    gyro_range_2000 = 3
};

enum mpu6050_accel_range {
    accel_range_2g = 0,
    accel_range_4g = 1,
    accel_range_8g = 2,
    accel_range_16g = 3
};


class MPU6050_Sensor: public IAccelerationSensor {
    public: 
        MPU6050_Sensor(II2C* i2c_bus, uint8_t i2c_addr=0x86, mpu6050_gyro_range gyro_range=gyro_range_1000, mpu6050_accel_range accel_range=accel_range_8g);
        void initialize();

        acceleration_struct get_imu_data();
    
    private:
        II2C *i2c_bus;
        uint8_t i2c_addr;
        mpu6050_gyro_range gyro_range;
        mpu6050_accel_range accel_range;

};

#endif
