#include "SimAccelerationSensor.h"

SimAccelerationSensor::SimAccelerationSensor() {

}

acceleration_struct SimAccelerationSensor::get_imu_data() {
    acceleration_struct a;
    a.accel_x = 0.1;
    a.accel_y = 0.2;
    a.accel_y = 0.3;
    a.gyro_x = 0.4;
    a.gyro_y = 0.5;
    a.gyro_z = 0.6;
    a.temperature = 21.0;
    return a;
}