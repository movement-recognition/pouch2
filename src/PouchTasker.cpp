#include <cstdio>
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
    xTaskCreate(
        (TaskFunction_t)[](void* _this){
            ((PouchTasker*) _this)->poll_imu_sensor();
        },
        "PollSensorTask", 1024, this, 2, NULL);
}

void PouchTasker::run() {
    vTaskStartScheduler();
}


void PouchTasker::poll_imu_sensor() {
    TickType_t xLastWakeTime;

    while(1) {
        xLastWakeTime = xTaskGetTickCount();
        printf("polling\n");
        printf("P1 %p\n", this->ptc);
        printf("P2 %p\n", this->ptc->imu_sensor);

        acceleration_struct foo = this->ptc->imu_sensor->get_imu_data();
        printf("structy\n");
        // printf("%jd\n", (uintmax_t)foo.accel_x);
        
        //vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
        vTaskDelay(pdMS_TO_TICKS(100));
        printf("after_task_delay\n");
    }
}

