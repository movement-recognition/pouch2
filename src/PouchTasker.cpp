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
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->poll_imu_sensor();
        },
        "PollSensorTask", 128, this, 2, NULL);
}

void PouchTasker::run() {
    vTaskStartScheduler();
}


void PouchTasker::poll_imu_sensor() {
    TickType_t xLastWakeTime;

    while(1) {
        xLastWakeTime = xTaskGetTickCount();

        // copy the dataset into new pointer
        std::unique_ptr<acceleration_struct> foo(new acceleration_struct(this->ptc->imu_sensor->get_imu_data()));

        xQueueSendToBack(this->message_queue_imu, (void *)foo.get(), 0);
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}

