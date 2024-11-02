#include <cstdio>
// #include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "lib/freeRTOS-Kernel/include/projdefs.h"
#include "FreeRTOS.h"
#include "task.h"

#include "PouchTasker.h"
#include "projdefs.h"


PouchTasker::PouchTasker(PouchTaskerConfig ptc) {
    this->ptc = ptc;
}


void PouchTasker::setup() {
    std::printf("setup!\n");
    xTaskCreate((TaskFunction_t)&PouchTasker::poll_imu_sensor, "PollSensorTask", 128, NULL, 2, NULL);
    printf("setup done.\n");
}

void PouchTasker::run() {
    printf("start run!\n");
    vTaskStartScheduler();
    printf("done running!\n");
}


void PouchTasker::poll_imu_sensor() {
    TickType_t xLastWakeTime;

    printf("start polling\n");

    for(;;) {
        xLastWakeTime = xTaskGetTickCount();
        printf("polling %d!", get_absolute_time());
        // acceleration_struct foo = this->ptc.imu_sensor->get_imu_data();
        
        //vTaskDelayUntil(&xLastWakeTime, this->ptc.imu_sensor_interval);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

