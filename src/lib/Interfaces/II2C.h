#ifndef II2C_H
#define II2C_H

#include <cstdint>
#include <vector>

class II2C {
    public:
        II2C() {};
        virtual ~II2C() {};

        virtual void print_scan() = 0;
        virtual std::vector<uint8_t> read_bytes(uint8_t addr, uint8_t register_addr, size_t length) = 0;
        virtual int write_bytes(uint8_t addr, uint8_t* bytes, size_t length) = 0;
};
#endif