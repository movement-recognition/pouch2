#include <stdio.h>
#include "pico/stdlib.h"
// for wifi-scanning
#include "pico/cyw43_arch.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"


#include "FreeRTOS.h"
#include "task.h"


bool wifiScanActive = false;

static int scan_result(void *env, const cyw43_ev_scan_result_t *result) {
    printf("scan_result called");
    if (result) {
        printf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n",
            result->ssid, result->rssi, result->channel,
            result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5],
            result->auth_mode);
    }
    return 0;
}

void vWifiScanTask() { // https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/wifi_scan/picow_wifi_scan.c
    for (;;) {

        if(!cyw43_wifi_scan_active(&cyw43_state)) {
            wifiScanActive = false;
        }
        if(wifiScanActive == false) {
            printf("initialize scan");
            wifiScanActive = true;
            cyw43_wifi_scan_options_t scan_options = {0};
            cyw43_wifi_scan(&cyw43_state, &scan_options, NULL, scan_result);
            printf("initialized scan");
        } 
        
        vTaskDelay(10000);
    }
}

void vWriteTask() {
    for (;;) {
        printf("HelloWorld!\n");
        vTaskDelay(500);
    }
}

int main() {
    stdio_init_all();

    for(int i = 0; i < 10; i++) {
        printf("count…%d\n", i);
        sleep_ms(1000);
    }    

    // initialize WiFi
    printf("init wiFi\n");
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }
    printf("enable_sta_mode\n");
    cyw43_arch_enable_sta_mode();
    printf("enabled sta_mode\n");
    
    
    xTaskCreate((TaskFunction_t)vWriteTask, "Write Task", 128, NULL, 2, NULL);
    xTaskCreate((TaskFunction_t)vWifiScanTask, "Blink Task", 512, NULL, 1, NULL);
    printf("StartScheduler\n");
    vTaskStartScheduler();

    printf("Started scheduler");

    cyw43_arch_deinit();
    return 0;
}