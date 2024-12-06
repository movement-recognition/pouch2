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
        uint32_t count_skipped_message = 0;
        uint32_t count_wrong_checksum = 0;

    private:
        uart_inst_t* uart_port;
        uint32_t uart_baudrate;
        char line_buffer[MAX_NMEA_BUFFER_LEN] = {0}; // see NMEA spec for max line length
        size_t line_ptr = 0;
        
        struct GNSS_raw { // remark: struct is packed to save space
            double latitude;
            double longitude;
            double altitude;
            uint64_t true_unix_time = 0; 
            uint64_t update_ts_latlon; // microcontroller ticks
            float heading;
            float speed;
            int32_t pdop;
            int32_t hdop;
            int32_t vdop;
            

            uint8_t gps_quality;
            bool validity;
            uint64_t update_ts_dop;  
        };

        // time references: https://gssc.esa.int/navipedia/index.php/Time_References_in_GNSS
        GNSS_raw* get_data_field(char* buffer);
        GNSS_raw GPS_data;
        GNSS_raw GLONASS_data;
        GNSS_raw GALILEO_data;
        GNSS_raw OTHER_data;
        
};
#endif