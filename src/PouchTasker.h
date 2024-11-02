#ifndef POUCHTASKER_H
#define POUCHTASKER
#include <functional>

#include "FreeRTOS.h"
#include "queue.h"
#include "IAccelerationSensor.h"

struct PouchTaskerConfig {
    IAccelerationSensor *imu_sensor;
    uint32_t imu_sensor_interval = 5; // time in milliseconds between sensor runs
    size_t imu_message_queue_length = 200; // equals one second of data
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

        QueueHandle_t message_queue_imu;
};
#endif