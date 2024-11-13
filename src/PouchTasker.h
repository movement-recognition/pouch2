#ifndef POUCHTASKER_H
#define POUCHTASKER
#include <functional>

#include "FreeRTOS.h"
#include "queue.h"
#include "IAccelerationSensor.h"
#include "IEnvironmentalSensor.h"

struct PouchTaskerConfig {
    IAccelerationSensor *imu_sensor;
    uint32_t imu_sensor_interval = 5; // time in milliseconds between sensor runs
    size_t imu_message_queue_length = 200; // equals one second of data

    IEnvironmentalSensor *env_sensor;
    uint32_t env_sensor_interval = 1000;
    size_t env_message_queue_length = 100;

    // IFilesystem* filesystem;
};

class PouchTasker {
    public:
        PouchTasker(PouchTaskerConfig *ptc_);
    
         
        void setup();
        void run();

        void poll_imu_sensor();
        void poll_environmental_sensor(); 

        void idle_core(uint8_t core_number);

    private:
        PouchTaskerConfig *ptc;

        QueueHandle_t message_queue_imu;

        TaskHandle_t poll_imu_task;
        TaskHandle_t poll_env_task;
        TaskHandle_t idle_core0_task;
        TaskHandle_t idle_core1_task;
};
#endif