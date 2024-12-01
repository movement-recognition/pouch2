#ifndef GPS_NMEA_H
#define GPS_NMEA_H

#include <cstdint>
#include <hardware/uart.h>

#include <pico/stdlib.h>
#include "lwgps/lwgps.h"

#include "IGlobalNavSatSystem.h"

const uint8_t MAX_NMEA_BUFFER_LEN = 85;
const uint32_t MAX_IDLE_TIME_MS = 5000;

class GPS_NMEA: public IGlobalNavSatSystem {
    public:
        GPS_NMEA(uart_inst_t* uart_port, size_t baud_rate, uint rx_pin, uint tx_pin);
        void poll();
        void restart();
        GNSS_position get_fix();
        uint64_t get_timestamp();

        lwgps_t handler; // TODO: make this handler private again

        uint64_t last_message = 0;
    private:
        uart_inst_t* uart_port;
        uint32_t uart_baudrate;
        char line_buffer[MAX_NMEA_BUFFER_LEN] = {0}; // see NMEA spec for max line length
        size_t line_ptr = 0;
        
        
};
#endif