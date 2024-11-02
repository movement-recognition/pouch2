#ifndef POUCHTASKER_H
#define POUCHTASKER
#include <functional>

#include "IAccelerationSensor.h"

struct PouchTaskerConfig {
    IAccelerationSensor *imu_sensor;
    uint32_t imu_sensor_interval = 5; // time in milliseconds between sensor runs

    // IFilesystem* filesystem;
};

class PouchTasker {
    public:
        PouchTasker(PouchTaskerConfig *ptc_);
    
         
        void setup();
        void run();

        void poll_imu_sensor();

    private:
        PouchTaskerConfig *ptc;
};
#endif