#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "FreeRTOS.h"
#include "task.h"

#include "II2C.h"
#include "I2C_Bus.h"

#include "IAccelerationSensor.h"
#include "Acceleration_MPU6050_Sensor.h"

void core1_main() {
    int i = 0;
    while(1) {
        printf("other core %d\n", i++);
        sleep_ms(1000);
    }
}

int main() {
    // init uart
    stdio_init_all();


    for(int i = 0; i<10; i++) {
        printf("booting…%d\n", i);
        sleep_ms(1000);
    }

    printf("launching multicore\n");
    multicore_launch_core1(core1_main);


    II2C *i2c_zero = new I2C_Bus(I2C_Bus_0, 100000U, true);

    i2c_zero->print_scan();
    // IAccelerationSensor *imu_sensor = new MPU6050_Sensor(i2c_zero, 0x86, gyro_range_1000, accel_range_4g);

    // acceleration_struct as = imu_sensor->get_imu_data();

    int i = 0;
    while(1) {
        printf("first core %d\n", i++);
        sleep_ms(600);
    }

    return 0;
}