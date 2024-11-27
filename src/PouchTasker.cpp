#include <cstdio>
#include <memory>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/multicore.h"

#include "lib/freeRTOS-Kernel/include/projdefs.h"
#include "FreeRTOS.h"
#include "task.h"

#include "PouchTasker.h"
#include "projdefs.h"


PouchTasker::PouchTasker(PouchTaskerConfig *ptc_) {
    this->ptc = ptc_;
}

void PouchTasker::setup() {

    // setup the IMU-Task
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->poll_imu_sensor();
        },
        "PollIMUTask", 256, this, 2, &(this->poll_imu_task));
    vTaskCoreAffinitySet(this->poll_imu_task, 0x01);
    
    // setup the environmental-task
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->poll_environmental_sensor();
        },
        "PollEnvironmentTask", 256, this, 2, &(this->poll_env_task));    
    vTaskCoreAffinitySet(this->poll_env_task, 0x01);

    // setup the eventqueue-to-sd-card-task
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->write_queue_to_sd();
        },
        "WriteQueueToSD", 512, this, 2, &(this->write_queue_to_sd_task));    
    vTaskCoreAffinitySet(this->write_queue_to_sd_task, 0x02);

    // setup HMI-task
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->update_hmi();
        },
        "UpdateHMI", 128, this, 2, &(this->update_hmi_task));    
    vTaskCoreAffinitySet(this->update_hmi_task, 0x02);


    // setup the idle task for core 0
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->idle_core(0);
        },
        "IdleCore0Task", 128, this, 1, &(this->idle_core0_task));
    vTaskCoreAffinitySet(this->idle_core0_task, 0x01);

    // setup the idle task for core 1
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->idle_core(1);
        },
        "IdleCore1Task", 128, this, tskIDLE_PRIORITY, &(this->idle_core1_task));
    vTaskCoreAffinitySet(this->idle_core1_task, 0x02);
}

void PouchTasker::run() {
    vTaskStartScheduler();
}


void PouchTasker::poll_imu_sensor() {
    TickType_t xLastWakeTime;

    while(true) {
        xLastWakeTime = xTaskGetTickCount();

        this->message_queue_imu.push(this->ptc->imu_sensor->get_imu_data());
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(this->ptc->imu_sensor_interval));
    }
}


void PouchTasker::poll_environmental_sensor() {
    TickType_t xLastWakeTime;

    while(true) {
        xLastWakeTime = xTaskGetTickCount();
        
        this->message_queue_env.push(this->ptc->env_sensor->get_environmental_data());
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(this->ptc->env_sensor_interval));
    }
}


void PouchTasker::write_queue_to_sd() {
    TickType_t xLastWakeTime;
    const size_t buffer_size = 100;
    char buffer[buffer_size];
    acceleration_struct imu_data;
    environment_struct env_data;

    this->ptc->sd_file_io->open_file("datalog.txt");
    while(true) {
        xLastWakeTime = xTaskGetTickCount();

        uint16_t len_queue_imu = this->message_queue_imu.size();
        if(len_queue_imu >= this->ptc->sd_card_write_batch_size) {
                        
            while(!this->message_queue_imu.empty()) {
                imu_data = this->message_queue_imu.front();
                snprintf(buffer, buffer_size-1, "IMU;%lld;%d;%d;%d;%d;%d;%d;%d\n", imu_data.timestamp, imu_data.accel_x, imu_data.accel_y, imu_data.accel_z, imu_data.gyro_x, imu_data.gyro_y, imu_data.gyro_z, imu_data.temperature);
                this->ptc->sd_file_io->write_line(buffer);
                this->message_queue_imu.pop();
            }
            this->ptc->sd_file_io->flush();
        }

        while(!this->message_queue_env.empty()) {
            env_data = this->message_queue_env.front();
            snprintf(buffer, buffer_size-1, "ENV;%lld;%d;%d;%d\n", env_data.timestamp, env_data.temperature, env_data.humidity, env_data.pressure);
            this->message_queue_env.pop();
        }
        this->ptc->sd_file_io->flush();


        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(this->ptc->sd_card_queue_check_interval));
    }
    this->ptc->sd_file_io->close_file();
}

void PouchTasker::update_hmi() {
    TickType_t xLastWakeTime;

    while(true) {
        xLastWakeTime = xTaskGetTickCount();

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }

}

void PouchTasker::idle_core(uint8_t core_number) {
    if(core_number == 0) {
        while(true) {
            // gpio_put(22, 0);
            // gpio_put(22, 1);
        }
    } else if(core_number == 1) {
        while(true) {
            // gpio_put(22, 0);
            // gpio_put(22, 1);
        }
    }
    
}

