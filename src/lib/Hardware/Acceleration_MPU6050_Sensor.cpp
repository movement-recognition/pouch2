#include <cstdint>
#include <cstdio>
#include "Acceleration_MPU6050_Sensor.h"

MPU6050_Sensor::MPU6050_Sensor(II2C* i2c_bus, uint8_t i2c_addr, mpu6050_gyro_range gyro_range, mpu6050_accel_range accel_range) {
    this->i2c_bus = i2c_bus;
    this->i2c_addr = i2c_addr;
    this->gyro_range = gyro_range;
    this->accel_range = accel_range;


    // configure MPU-sensor
    
    // register 25: sample rate divider
    // 0: 1kHz, 1: 500Hz, 2: 333Hz, 3: 250 Hz
    uint8_t samplerate_payload[] = {0x19, 0x00};
    this->i2c_bus->write_bytes(this->i2c_addr, samplerate_payload, 2);
    // register 26: activate low pass filter (184 Hz, F_s = 1kHz)
    uint8_t lowpass_payload[] = {0x1A, 0b00000000};
    this->i2c_bus->write_bytes(this->i2c_addr, lowpass_payload, 2);

    // gyroscope range
    uint8_t gyro_byte = (this->gyro_range << 3) & 0b00011000;
    uint8_t gyro_payload[] = {0x1B, gyro_byte};
    this->i2c_bus->write_bytes(this->i2c_addr, gyro_payload, 2);
    // accelerometer range
    uint8_t accel_byte = (this->accel_range << 3) & 0b00011000;
    uint8_t accel_payload[] = {0x1C, accel_byte};
    this->i2c_bus->write_bytes(this->i2c_addr, accel_payload, 2);

    // power management
    uint8_t power_payload[] = {0x6B, 0x00};
    this->i2c_bus->write_bytes(this->i2c_addr, power_payload, 2);
}


acceleration_struct MPU6050_Sensor::get_imu_data() {
    mpu6050_acceleration_struct raw_data_struct;

    const uint32_t lookup_accel[] = {16384, 8192, 4096, 2048};
    const uint32_t lookup_gyro[] = {131072, 65565, 32768, 16384};

    std::vector<uint8_t> data = this->i2c_bus->read_bytes(this->i2c_addr, 0x3B, 14);
    raw_data_struct.accel_x = data[0] << 8 | data[1];
    raw_data_struct.accel_y = data[2] << 8 | data[3];
    raw_data_struct.accel_z = data[4] << 8 | data[5];
    raw_data_struct.temperature = data[6] << 8 | data[7];
    raw_data_struct.gyro_x = data[8] << 8 | data[9];
    raw_data_struct.gyro_y = data[10] << 8 | data[11];
    raw_data_struct.gyro_z = data[12] << 8 | data[13];

    acceleration_struct data_struct;

    data_struct.accel_x = (int32_t)(raw_data_struct.accel_x * 1000) / lookup_accel[this->accel_range];
    data_struct.accel_y = (int32_t)(raw_data_struct.accel_y * 1000) / lookup_accel[this->accel_range];
    data_struct.accel_z = (int32_t)(raw_data_struct.accel_z * 1000) / lookup_accel[this->accel_range];

    data_struct.gyro_x = (int32_t)(raw_data_struct.gyro_x * 1000000) / lookup_gyro[this->gyro_range];
    data_struct.gyro_y = (int32_t)(raw_data_struct.gyro_y * 1000000) / lookup_gyro[this->gyro_range];
    data_struct.gyro_z = (int32_t)(raw_data_struct.gyro_z * 1000000) / lookup_gyro[this->gyro_range];

    data_struct.temperature = (int32_t)(raw_data_struct.temperature * 1000) / 340 + 36530;
    
    return data_struct;
}