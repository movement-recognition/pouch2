#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "II2C.h"
#include "I2C_Bus.h"

#include "IAccelerationSensor.h"
#include "Acceleration_MPU6050_Sensor.h"

int main() {

    // init uart
    stdio_init_all();

    II2C *i2c_zero = new I2C_Bus(I2C_Bus_0);

    // IAccelerationSensor *acceleration_sensor = new MPU6050_Sensor();

    // acceleration_sensor->initialize();

    i2c_zero->print_scan();


    return 0;
}