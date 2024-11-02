#include <stdio.h>
#include "pico/stdlib.h"

#include "II2C.h"
#include "I2C_Bus.h"

#include "PouchTasker.h"
#include "Acceleration_MPU6050_Sensor.h"

#include "SimAccelerationSensor.h"

int main() {
    // init uart
    stdio_init_all();

    for(int i = 0; i<5; i++) {
        printf("booting…%d\n", i);
        sleep_ms(1000);
    }
 
    II2C *i2c_zero = new I2C_Bus(I2C_Bus_0, 100000U, true);
    // i2c_zero->print_scan();

    PouchTaskerConfig *ptc = new PouchTaskerConfig();
    IAccelerationSensor *foo = new SimAccelerationSensor();
    printf("foo:\n");
    printf("x=%d\n", foo->get_imu_data().accel_x);
    printf("y=%d\n", foo->get_imu_data().accel_y);
    ptc->imu_sensor = new SimAccelerationSensor();//new MPU6050_Sensor(i2c_zero, 0x86, gyro_range_1000, accel_range_4g);
    printf("ptc:\n");
    auto data = ptc->imu_sensor->get_imu_data();
    printf("x=%d\n", data.accel_x);
    printf("y=%d\n", data.accel_y);
    ptc->imu_sensor_interval = 5;

    PouchTasker *pt = new PouchTasker(ptc);
    
    // acceleration_struct as = imu_sensor->get_imu_data();
    
    pt->setup();
    pt->run();

    foo->get_imu_data();
    return 0;
}