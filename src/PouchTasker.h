#ifndef POUCHTASKER_H
#define POUCHTASKER
#include <cstdint>
#include <functional>
#include <queue>

#include "FreeRTOS.h"
#include "IGlobalNavSatSystem.h"
#include "queue.h"
#include "IAccelerationSensor.h"
#include "IEnvironmentalSensor.h"
#include "IFileIO.h"

struct PouchTaskerConfig {
    IAccelerationSensor *imu_sensor;
    uint32_t imu_sensor_interval = 5; // time in milliseconds between sensor runs
    size_t imu_message_queue_length = 200; // equals one second of data

    IEnvironmentalSensor *env_sensor;
    uint32_t env_sensor_interval = 1000;
    size_t env_message_queue_length = 100;

    IFileIO* sd_file_io;
    uint32_t sd_card_queue_check_interval = 250;
    uint8_t sd_card_write_batch_size = 20; // 50: equals four writes a second

    IGlobalNavSatSystem* gps_sensor;
    uint32_t gps_sensor_interval = 50;
};

class PouchTasker {
    public:
        PouchTasker(PouchTaskerConfig *ptc_);
    
         
        void setup();
        void run();

        void poll_imu_sensor();
        void poll_environmental_sensor();
        void poll_gnss_sensor();
        void worker_gnss_sensor();

        void write_queue_to_sd();

        void update_hmi();

        void idle_core(uint8_t core_number);

    private:
        PouchTaskerConfig *ptc;

        TaskHandle_t poll_imu_task;
        std::queue<acceleration_struct> message_queue_imu;

        TaskHandle_t poll_env_task;
        std::queue<environment_struct> message_queue_env;

        TaskHandle_t worker_gnss_task;
        TaskHandle_t poll_gnss_task;
        std::queue<GNSS_position> message_queue_gnss;

        TaskHandle_t write_queue_to_sd_task;
        
        TaskHandle_t update_hmi_task;

        TaskHandle_t idle_core0_task;
        TaskHandle_t idle_core1_task;
};
#endif