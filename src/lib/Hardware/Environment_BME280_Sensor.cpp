#include <cstdint>
#include <cstdio>
#include "Environment_BME280_Sensor.h"
#include <pico/time.h>

BME280_Sensor::BME280_Sensor(II2C* i2c_bus, uint8_t i2c_addr) {
    this->i2c_bus = i2c_bus;
    this->i2c_addr = i2c_addr;

    // read calibration data
    std::vector<uint8_t> calibBlockA = this->i2c_bus->read_bytes(this->i2c_addr, 0x88, 26);
    std::vector<uint8_t> calibBlockB = this->i2c_bus->read_bytes(this->i2c_addr, 0xE1, 16);

    this->calib_dig_T1 = calibBlockA[1] << 8 | calibBlockA[0];
    this->calib_dig_T2 = calibBlockA[3] << 8 | calibBlockA[2];
    this->calib_dig_T3 = calibBlockA[5] << 8 | calibBlockA[4];

    this->calib_dig_P1 = calibBlockA[7] << 8 | calibBlockA[6];
    this->calib_dig_P2 = calibBlockA[9] << 8 | calibBlockA[8];
    this->calib_dig_P3 = calibBlockA[11] << 8 | calibBlockA[10];
    this->calib_dig_P4 = calibBlockA[13] << 8 | calibBlockA[12];
    this->calib_dig_P5 = calibBlockA[15] << 8 | calibBlockA[14];
    this->calib_dig_P6 = calibBlockA[17] << 8 | calibBlockA[16];
    this->calib_dig_P7 = calibBlockA[19] << 8 | calibBlockA[18];
    this->calib_dig_P8 = calibBlockA[21] << 8 | calibBlockA[20];
    this->calib_dig_P9 = calibBlockA[23] << 8 | calibBlockA[22];

    this->calib_dig_H1 = calibBlockA[25];
    this->calib_dig_H2 = calibBlockB[1] << 8 | calibBlockB[0];
    this->calib_dig_H3 = calibBlockB[2];
    this->calib_dig_H4 = ((int16_t)(int8_t)calibBlockB[3] * 16) | ((int16_t)(calibBlockB[4] & 0x0F));
    this->calib_dig_H5 = ((int16_t)(int8_t)calibBlockB[5] * 16) | ((int16_t)(calibBlockB[4] >> 4));
    this->calib_dig_H6 = (int8_t)calibBlockB[6];

    // set IIR-Filter settings
    uint8_t iir_filt[2] = {0xF5, 0b0010010};
    this->i2c_bus->write_bytes(this->i2c_addr, iir_filt, 1);
    // set the aquisition oversampling rate for humidity-measurement
    uint8_t hum_reg[2] = {0xF2, 0b00000001};
    this->i2c_bus->write_bytes(this->i2c_addr, hum_reg, 2);
    // set the aquisition oversampling rates for pressure and temperature
    uint8_t ctrl_reg[2] = {0xF4, 0b00100111}; // last two bytes: 11 = normal mode mode
    this->i2c_bus->write_bytes(this->i2c_addr, ctrl_reg, 2);
}


environment_struct BME280_Sensor::get_environmental_data() {
    environment_struct data;

    std::vector<uint8_t> raw_data = this->i2c_bus->read_bytes(this->i2c_addr, 0xF7, 8);
    uint32_t raw_pressure = raw_data[0] << 12 | raw_data[1] << 4 | raw_data[2];
    uint32_t raw_temperature = raw_data[3] << 12 | raw_data[4] << 4 | raw_data[5];
    uint32_t raw_humidity = raw_data[6] << 8 | raw_data[7];
    data.temperature = this->compensate_temperature(raw_temperature);
    data.pressure = this->compensate_pressure(raw_pressure);
    data.humidity = this->compensate_humidity(raw_humidity);

    printf("temp=%d\tpres=%d\thudy=%d\n", data.temperature, data.pressure, data.humidity);
    return data;
}

int32_t BME280_Sensor::compensate_temperature(const int32_t raw_temperature) {
    int32_t var1;
    int32_t var2;
    int32_t temperature;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;

    var1 = (int32_t)((raw_temperature / 8) - ((int32_t)this->calib_dig_T1 * 2));
    var1 = (var1 * ((int32_t)this->calib_dig_T2)) / 2048;
    var2 = (int32_t)((raw_temperature / 16) - ((int32_t)this->calib_dig_T1));
    var2 = (((var2 * var2) / 4096) * ((int32_t)this->calib_dig_T3)) / 16384;
    this->calib_t_fine = var1 + var2;
    temperature = (this->calib_t_fine * 5 + 128) / 256;

    if (temperature < temperature_min) {
        temperature = temperature_min;
    } else if (temperature > temperature_max) {
        temperature = temperature_max;
    }

    return temperature;
}

uint32_t BME280_Sensor::compensate_pressure(const int32_t raw_pressure) {
    int64_t var1;
    int64_t var2;
    int64_t var3;
    int64_t var4;
    uint32_t pressure;
    uint32_t pressure_min = 3000000;
    uint32_t pressure_max = 11000000;

    var1 = ((int64_t)this->calib_t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)this->calib_dig_P6;
    var2 = var2 + ((var1 * (int64_t)this->calib_dig_P5) * 131072);
    var2 = var2 + (((int64_t)this->calib_dig_P4) * 34359738368);
    var1 = ((var1 * var1 * (int64_t)this->calib_dig_P3) / 256) + ((var1 * ((int64_t)this->calib_dig_P2) * 4096));
    var3 = ((int64_t)1) * 140737488355328;
    var1 = (var3 + var1) * ((int64_t)this->calib_dig_P1) / 8589934592;

    /* To avoid divide by zero exception */
    if (var1 != 0)
    {
        var4 = 1048576 - raw_pressure;
        var4 = (((var4 * INT64_C(2147483648)) - var2) * 3125) / var1;
        var1 = (((int64_t)this->calib_dig_P9) * (var4 / 8192) * (var4 / 8192)) / 33554432;
        var2 = (((int64_t)this->calib_dig_P8) * var4) / 524288;
        var4 = ((var4 + var1 + var2) / 256) + (((int64_t)this->calib_dig_P7) * 16);
        pressure = (uint32_t)(((var4 / 2) * 100) / 128);

        if (pressure < pressure_min)
        {
            pressure = pressure_min;
        }
        else if (pressure > pressure_max)
        {
            pressure = pressure_max;
        }
    }
    else
    {
        pressure = pressure_min;
    }

    return pressure;
}

uint32_t BME280_Sensor::compensate_humidity(const uint32_t raw_humidity) {
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t var4;
    int32_t var5;
    uint32_t humidity;
    uint32_t humidity_max = 102400;

    var1 = this->calib_t_fine - ((int32_t)76800);
    var2 = (int32_t)(raw_humidity * 16384);
    var3 = (int32_t)(((int32_t)this->calib_dig_H4) * 1048576);
    var4 = ((int32_t)this->calib_dig_H5) * var1;
    var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    var2 = (var1 * ((int32_t)this->calib_dig_H6)) / 1024;
    var3 = (var1 * ((int32_t)this->calib_dig_H3)) / 2048;
    var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    var2 = ((var4 * ((int32_t)this->calib_dig_H2)) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((int32_t)this->calib_dig_H1)) / 16);
    var5 = (var5 < 0 ? 0 : var5);
    var5 = (var5 > 419430400 ? 419430400 : var5);
    humidity = (uint32_t)(var5 / 4096);

    if (humidity > humidity_max) {
        humidity = humidity_max;
    }

    return humidity;
}