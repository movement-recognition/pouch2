#include <stdexcept>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "I2C_Bus.h"

I2C_Bus::I2C_Bus(I2C_Bus_Number bus_number, uint32_t baud_rate, bool pullups) {
    uint8_t sda_pin;
    uint8_t scl_pin;
    if(bus_number == I2C_Bus_0) {
        this->i2c_number = i2c0;
    } else if(bus_number == I2C_Bus_1) {
        this->i2c_number = i2c1;
        throw std::invalid_argument("using the second i2c-hardware not implemented yet");
    } else {
        throw std::invalid_argument("no correct i2c-bus-number specified");
    }

    i2c_init(i2c_number, baud_rate);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    if(pullups) {
        gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
        gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    }

    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
}

bool I2C_Bus::reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void I2C_Bus::print_scan() {
    // See official documentation in pico-sdk

    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(this->i2c_number, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
}