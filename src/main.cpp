#include <cstdint>
#include <hardware/uart.h>
#include <pico/time.h>
#include <stdio.h>
#include <pico/stdlib.h>

#include "II2C.h"
#include "I2C_Bus.h"
#include "IFileIO.h"

#include "PouchTasker.h"
#include "Acceleration_MPU6050_Sensor.h"
#include "Environment_BME280_Sensor.h"
#include "SDCardSocket.h"
#include "GPS_NMEA.h"

int main() {

     // for now only used for debugging // TODO: remove
    gpio_init(20);
    gpio_set_dir(20, GPIO_OUT);
    gpio_init(21);
    gpio_set_dir(21, GPIO_OUT);
    gpio_init(22);
    gpio_set_dir(22, GPIO_OUT);


    // init uart
    stdio_init_all();
    stdio_usb_init();
    // Setup the UART0 port as a seperate item
    #define UART0_ID        uart0
    #define UART0_BAUD_RATE 115200
    #define UART0_TX_GP    0
    #define UART0_RX_GP    1
    uart_init(UART0_ID, UART0_BAUD_RATE);
    gpio_set_function(UART0_TX_GP, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_GP, GPIO_FUNC_UART);


    for(int i = 0; i<5; i++) {
        printf("booting…%d\n", i);
        sleep_ms(500);
    }

    GPS_NMEA foo = GPS_NMEA(uart1, 9600, 9, 8);
    sleep_ms(10);
    printf("while loop\n");
    while(true) {
        gpio_put(22, 0);
        foo.poll();
        gpio_put(22, 1);
        uint64_t delta = to_us_since_boot(get_absolute_time()) - foo.last_message;
        if(delta > 5000000 ) {
            printf("restart delta!\n");
            foo.restart();
        }
    }
    
    return 0;

    // DEBUG

    // TODO: replace all new()-initializiations with unique_ptr!

    printf("initializing sd_card\n");
    IFileIO *io_storage = new SDCardSocket(true);
    printf("initialized sd_card\n");
    sleep_ms(10);
    io_storage->mount_card();
    printf("mounted sd_card\n");
    printf("writing test data to file\n");
    io_storage->open_file("test.txt");
    io_storage->write_line("foobar test\n");
    io_storage->close_file();
    printf("wrote test data to file.\n");

    printf("initializing i2c-bus\n");
    II2C *i2c_zero = new I2C_Bus(I2C_Bus_1, 6, 7, 400000U, true);
    i2c_zero->print_scan();
    printf("initialized i2c-bus\n");


    printf("setup of all peripherals\n");
    PouchTaskerConfig *ptc = new PouchTaskerConfig();
    ptc->imu_sensor = new MPU6050_Sensor(i2c_zero, 0x68, gyro_range_1000, accel_range_2g); // new SimAccelerationSensor();
    ptc->imu_sensor_interval = 5;

    ptc->env_sensor = new BME280_Sensor(i2c_zero, 0x77);
    ptc->env_sensor_interval = 500;
    
    ptc->gps_sensor = new GPS_NMEA(uart1, 9600, 9, 8);
    ptc->gps_sensor_interval = 50; // TODO: refine this interval as the sensors outputs data all the time!

    ptc->sd_file_io = io_storage;

    printf("peripherals setup complete.\n");

    // DEBUG Playground
    // END DEBUG Playground


    printf("setup pouch tasker\n");
    PouchTasker *pt = new PouchTasker(ptc);
    
    pt->setup();
    printf("run tasker\n");
    pt->run();

    while(true) {
        asm("nop");
    }
    printf("exited tasker\n");
    return 0;
}