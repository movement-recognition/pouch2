#ifndef GPS_NMEA_H
#define GPS_NMEA_H

#include <cstdint>
#include <map>
#include <hardware/uart.h>

#include <pico/stdlib.h>

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

        uint64_t last_message = 0;
        std::map<GNSS_constellation, uint64_t> count_message;

    private:
        uart_inst_t* uart_port;
        uint32_t uart_baudrate;
        char line_buffer[MAX_NMEA_BUFFER_LEN] = {0}; // see NMEA spec for max line length
        size_t line_ptr = 0;
        
        struct GNSS_raw { // TODO: struct-packing to save some space?
            // GLL-Message
            double latitude;
            double longitude;
            bool validity;
            uint64_t update_ts_latlon;
            // GGA-Message (extends info from GLL-message)
            uint8_t gps_quality;
            double altitude;

            // GSA-Message
            int32_t pdop;
            int32_t hdop;
            int32_t vdop;
            uint64_t update_ts_dop;
            
        };

        GNSS_raw GPS_data;
        GNSS_raw GLONASS_data;
        GNSS_raw GALILEO_data;
        
};
#endif