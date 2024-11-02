#include "SimAccelerationSensor.h"

SimAccelerationSensor::SimAccelerationSensor() {

}

acceleration_struct SimAccelerationSensor::get_imu_data() {
    acceleration_struct a;
    a.accel_x = 1;
    a.accel_y = 2;
    a.accel_y = 3;
    a.gyro_x = 4;
    a.gyro_y = 5;
    a.gyro_z = 6;
    a.temperature = 21;
    return a;
}