#include <cstdio>
#include <memory>
#include "pico/stdlib.h"
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
    // setup the Measurement-Queue
    this->message_queue_imu = xQueueCreate(this->ptc->imu_message_queue_length, sizeof(std::unique_ptr<acceleration_struct>));

    // setup the IMU-Task
    /*xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->poll_imu_sensor();
        },
        "PollIMUTask", 128, this, 2, NULL);
    */
    // setup the environmental-task
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->poll_environmental_sensor();
        },
        "PollEnvironmentTask", 128, this, 2, NULL);
}

void PouchTasker::run() {
    vTaskStartScheduler();
}


void PouchTasker::poll_imu_sensor() {
    TickType_t xLastWakeTime;

    while(true) {
        xLastWakeTime = xTaskGetTickCount();

        // copy the dataset into new pointer
        std::unique_ptr<acceleration_struct> acceleration_data(new acceleration_struct(this->ptc->imu_sensor->get_imu_data()));
        printf("a_data x=%d\ty=%d\tz=%d\n", acceleration_data->accel_x, acceleration_data->accel_y, acceleration_data->accel_z);
        // xQueueSendToBack(this->message_queue_imu, (void *)acceleration_data.get(), 0);
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(this->ptc->imu_sensor_interval));
    }
}




void PouchTasker::poll_environmental_sensor() {
    TickType_t xLastWakeTime;

    while(true) {
        xLastWakeTime = xTaskGetTickCount();

        // copy the dataset into new pointer
        std::unique_ptr<environment_struct> environmental_data(new environment_struct(this->ptc->env_sensor->get_environmental_data()));
        // printf("a_data x=%d\ty=%d\tz=%d\n", acceleration_data->accel_x, acceleration_data->accel_y, acceleration_data->accel_z);
        // xQueueSendToBack(this->message_queue_imu, (void *)acceleration_data.get(), 0);
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(this->ptc->env_sensor_interval));
    }
}

