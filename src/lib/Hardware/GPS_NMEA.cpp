#include <string.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/uart.h>
#include "lwgps/lwgps.h"

#include "IGlobalNavSatSystem.h"
#include "lwgps/lwgps.h"
#include "GPS_NMEA.h"

GPS_NMEA::GPS_NMEA(uart_inst_t* uart_port_, size_t baud_rate_, uint rx_pin_, uint tx_pin_) {
    printf("init GPS\n");
    this->uart_port = uart_port_;
    //gpio_set_dir(tx_pin, GPIO_IN);
    gpio_set_dir(rx_pin_, GPIO_IN);
    //gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin_, GPIO_FUNC_UART);
    uart_init(this->uart_port, baud_rate_);

    lwgps_init(&this->handler);
    printf("init gps done\n");
}

void GPS_NMEA::poll() {
    while(!uart_is_readable(this->uart_port)) {}

    while(uart_is_readable(this->uart_port)) {
        char recent_char = uart_getc(this->uart_port);
        this->line_buffer[this->line_ptr++] = recent_char;
        if(recent_char == '\r') {
            this->line_buffer[this->line_ptr] = 0;
            lwgps_process(&this->handler, this->line_buffer, strlen(this->line_buffer));
            this->line_ptr = 0;

            this->get_fix();
            return;
        }
    }
}

GNSS_position GPS_NMEA::get_fix() {
    printf("get fix!\n");
    GNSS_position d;
    d.latitude = this->handler.latitude;
    d.longitude = this->handler.longitude;
    d.altitude = this->handler.altitude;
    d.timestamp = to_us_since_boot(get_absolute_time());
    printf("fix?=%u sat_in_view=%u  sat_in_use=%u\n", this->handler.fix, this->handler.sats_in_view, this->handler.sats_in_use);
    printf("lat=%f  lon=%f  alt=%f \n", this->handler.latitude, this->handler.longitude, this->handler.altitude);
    printf("fix done!\n");
    return d;
}

uint64_t GPS_NMEA::get_timestamp() {
    return 0;
}