#ifndef GPS_NMEA_H
#define GPS_NMEA_H

#include <cstdint>
#include <hardware/uart.h>

#include <pico/stdlib.h>
#include "lwgps/lwgps.h"

#include "IGlobalNavSatSystem.h"

class GPS_NMEA: public IGlobalNavSatSystem {
    public:
        GPS_NMEA(uart_inst_t* uart_port, size_t baud_rate, uint rx_pin, uint tx_pin);
        void poll();
        GNSS_position get_fix();
        uint64_t get_timestamp();

        lwgps_t handler; // TODO: make this handler private again
    private:
        uart_inst_t* uart_port;
        char line_buffer[85] = {0}; // see NMEA spec for max line length
        size_t line_ptr = 0;
        
};
#endif