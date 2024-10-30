#ifndef I2C_BUS_H
#define I2C_BUS_H
#include <cstdint>
#include "hardware/i2c.h"

#include "II2C.h"

enum I2C_Bus_Number {I2C_Bus_0, I2C_Bus_1};

class I2C_Bus: public II2C {

    public:
        I2C_Bus(I2C_Bus_Number bus_number, uint32_t baud_rate=100*1000);

        void print_scan();

    private:
        i2c_inst_t *i2c_number;
        bool reserved_addr(uint8_t addr);

};

#endif