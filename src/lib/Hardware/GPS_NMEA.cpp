#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <pico/time.h>
#include <string.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/uart.h>
#include "lwgps/lwgps.h"

#include "IGlobalNavSatSystem.h"
#include "lwgps/lwgps.h"
#include "GPS_NMEA.h"

bool nmea_compare_msg_prefix(const char* buffer, const char* prefix) {
    bool check = true;
    check &= buffer[3] == prefix[0];
    check &= buffer[4] == prefix[1];
    check &= buffer[5] == prefix[2];
    return check;
}

uint8_t ascii_to_hex(char ascii) {
    if(ascii >= 0x30 && ascii <= 0x39) {
        return ascii - 0x30;
    } else if(ascii >= 0x41 && ascii <= 0x46) {
        return ascii - 0x37;
    } else {
        return 0x00;
    }
}

bool nmea_checksum(const char* buffer, const size_t buffer_length) {
    if(buffer[buffer_length-3] != '*') {
        return false;
    }
    uint8_t checksum_recv = (ascii_to_hex(buffer[buffer_length-2]) << 4);
    checksum_recv += ascii_to_hex(buffer[buffer_length-1]);

    uint8_t checksum_msg = buffer[1];
    for(size_t i = 2; i < buffer_length-3; i++) {
        checksum_msg ^= buffer[i];
    }

    return checksum_msg == checksum_recv;
}


GPS_NMEA::GPS_NMEA(uart_inst_t* uart_port_, size_t baud_rate_, uint rx_pin_, uint tx_pin_) {
    printf("init GPS\n");
    this->uart_port = uart_port_;
    this->uart_baudrate = baud_rate_;
    //gpio_set_dir(tx_pin, GPIO_IN);
    gpio_set_dir(rx_pin_, GPIO_IN);
    //gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin_, GPIO_FUNC_UART);
    uart_init(this->uart_port, this->uart_baudrate);

    lwgps_init(&this->handler);
    printf("init gps done\n");
}

void GPS_NMEA::restart() {
    printf("restarting uart1!\n");
    uart_deinit(this->uart_port);
    sleep_ms(1);
    uart_init(this->uart_port, this->uart_baudrate);
}

void GPS_NMEA::poll() {
    uint8_t state = 0;
    while(uart_is_readable_within_us(this->uart_port, 1000)) {
        char recent_char = uart_getc(this->uart_port);
        if(recent_char == '$') {
            this->line_ptr = 0;
            state = 1;
        }
        if(recent_char != '\n' && recent_char != '\r') {
            this->line_buffer[(this->line_ptr++)%MAX_NMEA_BUFFER_LEN] = recent_char;
            if(state == 1 && recent_char != '$') {
                state = 2;
            }
        } else if(recent_char == '\n' || recent_char == '\r' || (recent_char == '$' && state == 2)) {
            state = 0;
            this->line_buffer[this->line_ptr] = 0;
            
            if(this->line_ptr == 0 || this->line_buffer[0] != '$') { // skip incorreclty parsed lines
                return;
            } else if(!nmea_checksum(this->line_buffer, this->line_ptr)) { // no correct checksum
                printf("message with wrong checksum received.\n");
                return;
            }
            // parse line
            printf("ptr=%u\t_%s\n", this->line_ptr, this->line_buffer);
            this->last_message = to_us_since_boot(get_absolute_time());
            const char *delim = ",";
            if(nmea_compare_msg_prefix(this->line_buffer, "GLL")) {
                
                char *t = strtok(this->line_buffer, delim);
                size_t segment = 0;
                double lat = 0.0;
                char lat_ns = '\0';
                double lon = 0.0;
                char lon_ew = '\0';
                double time = 0.0;
                char state = '\0';
                while(t != nullptr) {
                    switch (segment) {
                        case 1:
                            lat = std::strtof(t, nullptr);
                            break;
                        case 2:
                            lat_ns = t[0];
                            break;
                        case 3:
                            lon = std::strtof(t, nullptr);
                            break;
                        case 4:
                            lon_ew = t[0];
                            break;
                        case 5:
                            time = std::strtof(t, nullptr);
                            break;
                        case 6:
                            state = t[0];
                            break;

                    }

                    t = strtok(nullptr, delim); 
                    segment++;
                }

                printf("lat=%f  lon=%f  state=%d (%c/%c)\n", lat, lon, state, lon_ew, lat_ns);
                printf("%s\n", this->line_buffer);
            } else {
                printf("other %s\n", this->line_buffer);
            }

            this->line_ptr = 0;
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