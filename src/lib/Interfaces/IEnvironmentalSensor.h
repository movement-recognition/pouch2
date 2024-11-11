#ifndef IENVIRONMENTALSENSOR_H
#define IENVIRONMENTALSENSOR_H

#include "II2C.h"

struct environment_struct {
    // fixed point: all values 1000* (in Kelvin, percent etc.)
    int32_t temperature = 0;
    int32_t humidity = 0;
    int32_t pressure = 0;
};

class IEnvironmentalSensor {
    public:
        IEnvironmentalSensor() {};
        virtual ~IEnvironmentalSensor() {};

        virtual environment_struct get_environmental_data() = 0;

};

#endif