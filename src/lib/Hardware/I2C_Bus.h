#ifndef I2C_BUS_H
#define I2C_BUS_H
#include <cstdint>
#include "pico/multicore.h"
#include "hardware/i2c.h"

#include "II2C.h"

enum I2C_Bus_Number {I2C_Bus_0, I2C_Bus_1};


class I2C_Bus: public II2C {

    public:
        I2C_Bus(I2C_Bus_Number bus_number, uint8_t sda_pin, uint8_t scl_pin, uint32_t baud_rate=100*1000, bool pullups=false);

        void print_scan();
        std::vector<uint8_t> read_bytes(uint8_t addr, uint8_t register_addr, size_t length);
        int write_bytes(uint8_t addr, uint8_t* bytes, size_t length);

    private:
        i2c_inst_t *i2c_number;
        bool reserved_addr(uint8_t addr);
        mutex_t lock;
        // auto_init_mutex(lock);
};

#endif