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


    for(int i = 0; i<10; i++) {
        printf("booting…%d\n", i);
        sleep_ms(1000);
    }



    II2C *i2c_zero = new I2C_Bus(I2C_Bus_0);
    // IAccelerationSensor *acceleration_sensor = new MPU6050_Sensor();

    // acceleration_sensor->initialize();


    return 0;
}