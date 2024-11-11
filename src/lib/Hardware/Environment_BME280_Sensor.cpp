#include <cstdint>
#include <cstdio>
#include "Environment_BME280_Sensor.h"

BME280_Sensor::BME280_Sensor(II2C* i2c_bus, uint8_t i2c_addr) {
    this->i2c_bus = i2c_bus;
    this->i2c_addr = i2c_addr;

    // read calibration data
    std::vector<uint8_t> calibBlockA = this->i2c_bus->read_bytes(this->i2c_addr, 0x88, 26);
    std::vector<uint8_t> calibBlockB = this->i2c_bus->read_bytes(this->i2c_addr, 0xE1, 16);
    size_t a = 0;
    for(uint8_t q: calibBlockA) {
        this->calibration_data[a++] = q;
    }
    for(uint8_t q: calibBlockB) {
        this->calibration_data[a++] = q;
    }
}

environment_struct BME280_Sensor::get_environmental_data() {
    environment_struct data;
    std::vector<uint8_t> status_flag = this->i2c_bus->read_bytes(this->i2c_addr, 0xD0, 1);
    printf("flagD0=%d\n", status_flag[0]);

    std::vector<uint8_t> meas = this->i2c_bus->read_bytes(this->i2c_addr, 0xF3, 1);
    printf("meas=%d\n", meas[0]);

    // set the aquisition oversampling rates
    uint8_t ctrl_reg[2] = {0xF4, 0b00100111};
    this->i2c_bus->write_bytes(this->i2c_addr, ctrl_reg, 1);


    std::vector<uint8_t> raw_data = this->i2c_bus->read_bytes(this->i2c_addr, 0xF7, 8);
    uint32_t raw_pressure = raw_data[0] << 12 | raw_data[1] << 4 | raw_data[2];
    uint32_t raw_temperature = raw_data[3] << 12 | raw_data[4] << 4 | raw_data[5];
    uint32_t raw_humidity = raw_data[6] << 8 | raw_data[7];
    printf("pres=%d\ttemp=%d\thudy=%d\n", raw_pressure, raw_temperature, raw_humidity);
    return data;
}