#include <hardware/uart.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <stdio.h>
#include <string.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>

#include "IGlobalNavSatSystem.h"
#include "GPS_NMEA.h"

bool nmea_compare_msg_prefix(const char *buffer, const char *prefix) {
    bool check = true;
    check &= buffer[3] == prefix[0];
    check &= buffer[4] == prefix[1];
    check &= buffer[5] == prefix[2];
    return check;
}

GNSS_constellation nmea_get_msg_constellation(const char *buffer) {
    if (buffer[1] == 'G' && buffer[2] == 'P') {
        return GNSS_constellation::GPS;
    } else if (buffer[1] == 'G' && buffer[2] == 'A') {
        return GNSS_constellation::GALILEO;
    } else if (buffer[1] == 'G' && buffer[2] == 'L') {
        return GNSS_constellation::GLONASS;
    } else if (buffer[1] == 'G' && buffer[2] == 'B') {
        return GNSS_constellation::BEIDOU;
    } else if (buffer[1] == 'G' && buffer[2] == 'N') {
        return GNSS_constellation::COMBINED;
    } else {
        return GNSS_constellation::UNKNOWN;
    };
}

uint8_t ascii_to_hex(char ascii) {
    if (ascii >= 0x30 && ascii <= 0x39) {
        return ascii - 0x30;
    } else if (ascii >= 0x41 && ascii <= 0x46) {
        return ascii - 0x37;
    } else {
        return 0x00;
    }
}

bool nmea_checksum(const char *buffer, const size_t buffer_length) {
    if (buffer[buffer_length - 3] != '*') {
        return false;
    }
    uint8_t checksum_recv = (ascii_to_hex(buffer[buffer_length - 2]) << 4);
    checksum_recv += ascii_to_hex(buffer[buffer_length - 1]);

    uint8_t checksum_msg = buffer[1];
    for (size_t i = 2; i < buffer_length - 3; i++) {
        checksum_msg ^= buffer[i];
    }

    return checksum_msg == checksum_recv;
}

GPS_NMEA::GPS_NMEA(uart_inst_t *uart_port_, size_t baud_rate_, uint rx_pin_, uint tx_pin_) {
    printf("init GPS\n");
    this->uart_port = uart_port_;
    this->uart_baudrate = baud_rate_;
    // gpio_set_dir(tx_pin, GPIO_IN);
    gpio_set_dir(rx_pin_, GPIO_IN);
    // gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin_, GPIO_FUNC_UART);
    uart_init(this->uart_port, this->uart_baudrate);

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
    while (uart_is_readable_within_us(this->uart_port, 1000)) {
        char recent_char = uart_getc(this->uart_port);
        if (recent_char == '$') {
            this->line_ptr = 0;
            state = 1;
        }
        if (recent_char != '\n' && recent_char != '\r') {
            this->line_buffer[(this->line_ptr++) % MAX_NMEA_BUFFER_LEN] = recent_char;
            if (state == 1 && recent_char != '$') {
                state = 2;
            }
        } else if (recent_char == '\n' || recent_char == '\r' || (recent_char == '$' && state == 2)) {
            state = 0;
            this->line_buffer[this->line_ptr] = 0;

            if (this->line_ptr == 0 || this->line_buffer[0] != '$') {  // skip incorreclty parsed lines
                this->count_skipped_message++;
                return;
            } else if (!nmea_checksum(this->line_buffer,
                                      this->line_ptr)) {  // no correct checksum
                this->count_wrong_checksum++;
                return;
            }
            // earliest point in time of the message timestamp
            uint64_t capture_time = to_us_since_boot(get_absolute_time());
            // start line processing:

            // printf("\rptr=%u\t_%s\n", this->line_ptr, this->line_buffer);
            GNSS_constellation line_constellation = nmea_get_msg_constellation(this->line_buffer);
            // statistics
            this->last_message = to_us_since_boot(get_absolute_time());
            this->count_message[line_constellation]++;

            // parse line
            const char *delim = ",";
            size_t segment = 0;
            char *t = strtok(this->line_buffer, delim);
            if (nmea_compare_msg_prefix(this->line_buffer, "GLL")) {
                // $GNGLL,4927.8237,N,00829.3764,E,160355.000,A,A*40
                float hhmmss_raw;      // date in weird decimal format
                char receiver_status;  // A = signal is valid, V = receiver warning
                double lat = 0.0;      // in hundredths of a degree
                char lat_ns = '\0';    // 'N' or 'S'
                double lon = 0.0;      // in hundredths of a degree
                char lon_ew = '\0';    // 'E' or 'W'
                while (t != nullptr) {
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
                            hhmmss_raw = std::strtof(t, nullptr);
                            break;
                        case 6:
                            receiver_status = t[0];
                            break;
                    }
                    t = strtok(nullptr, delim);
                    segment++;
                }
                // switch the data dumping to the correct sub-structure
                GNSS_raw *data = get_data_field(line_constellation);

                // reprocess coordinates
                if (lat_ns == 'N') {
                    data->latitude = lat / 100.0;
                } else if (lat_ns == 'S') {
                    data->latitude = lat / -100.0;
                } else {
                    data->latitude = 0.0;
                }
                if (lon_ew == 'E') {
                    data->longitude = lon / 100.0;
                } else if (lon_ew == 'W') {
                    data->longitude = lon / -100.0;
                } else {
                    data->longitude = 0.0;
                }

                // reprocess time
                std::tm tm{};
                const int hhmmss_rawint = hhmmss_raw;
                tm.tm_sec = hhmmss_rawint % 100;
                tm.tm_min = ((hhmmss_rawint - (hhmmss_rawint % 100)) % 10000) / 100;
                tm.tm_hour = (hhmmss_rawint - (hhmmss_rawint % 10000)) / 10000;
                tm.tm_isdst = 0;

                std::time_t time = std::mktime(&tm);

                data->true_unix_time = time;
                data->update_ts_latlon = capture_time;
            } else if (nmea_compare_msg_prefix(this->line_buffer, "GGA")) {
                // $GPGGA,160353.000,4927.8229,N,00829.3770,E,1,4,3.19,83.8,M,47.9,M,,*61
            } else if (nmea_compare_msg_prefix(this->line_buffer, "GSA")) {
                // $GNGSA,A,3,18,28,31,26,,,,,,,,,4.98,4.90,0.89,1*0D
                // $GNGSA,A,3,68,,,,,,,,,,,,4.98,4.90,0.89,2*05
            } else if (nmea_compare_msg_prefix(this->line_buffer, "GSV")) {
                // $GPGSV,3,1,11,29,78,210,17,25,67,095,,28,59,275,29,31,36,306,26,0*67
                // $GPGSV,3,2,11,11,28,051,,12,25,097,,26,17,289,18,18,12,177,,0*63
                // $GPGSV,3,3,11,20,11,076,,32,11,233,,06,01,025,,0*55
                // $GLGSV,2,1,08,86,62,066,,76,55,109,,87,52,174,,68,17,311,21,1*72
                // $GLGSV,2,2,08,85,13,029,,78,12,308,,69,09,359,,67,05,266,,1*7C
            } else if (nmea_compare_msg_prefix(this->line_buffer, "VTG")) {
                // $GPVTG,111.46,T,,M,0.00,N,0.00,K,A*3E
            } else if (nmea_compare_msg_prefix(this->line_buffer, "RMC")) {
                // $GNRMC,192123.000,A,4927.8201,N,00829.4121,E,0.98,107.08,061224,,,A,V*0A
                float hhmmss_raw;           // date in weird decimal format
                char receiver_status;       // A = signal is valid, V = receiver warning
                double lat = 0.0;           // in hundredths of a degree
                char lat_ns = '\0';         // 'N' or 'S'
                double lon = 0.0;           // in hundredths of a degree
                char lon_ew = '\0';         // 'E' or 'W'
                double ground_speed = 0.0;  // unit: knots
                double heading = 0.0;       // heading in degrees
                int32_t ddmmyy_raw;         // date in weird decimal format
                while (t != nullptr) {
                    switch (segment) {
                        case 1:
                            hhmmss_raw = std::strtof(t, nullptr);
                            break;
                        case 2:
                            receiver_status = t[0];
                            break;
                        case 3:
                            lat = std::strtof(t, nullptr);
                            break;
                        case 4:
                            lat_ns = t[0];
                            break;
                        case 5:
                            lon = std::strtof(t, nullptr);
                            break;
                        case 6:
                            lon_ew = t[0];
                            break;
                        case 7:
                            ground_speed = std::strtof(t, nullptr);
                            break;
                        case 8:
                            heading = std::strtof(t, nullptr);
                            break;
                        case 9:
                            ddmmyy_raw = std::strtof(t, nullptr);
                            break;
                    }
                    t = strtok(nullptr, delim);
                    segment++;
                }
                // only update handler if state is valid ('A')
                if (receiver_status == 'A') {
                    // switch the data dumping to the correct sub-structure
                    GNSS_raw *data = get_data_field(line_constellation);                   

                    // reprocess coordinates
                    if (lat_ns == 'N') {
                        data->latitude = lat / 100.0;
                    } else if (lat_ns == 'S') {
                        data->latitude = lat / -100.0;
                    } else {
                        data->latitude = 0.0;
                    }
                    if (lon_ew == 'E') {
                        data->longitude = lon / 100.0;
                    } else if (lon_ew == 'W') {
                        data->longitude = lon / -100.0;
                    } else {
                        data->longitude = 0.0;
                    }

                    data->speed = ground_speed * 0.5144444;  // convert knots to m/s
                    data->heading = heading;

                    // reprocess time
                    std::tm tm{};
                    tm.tm_year = 2000 + ddmmyy_raw % 100;
                    tm.tm_mon = (ddmmyy_raw % 10000 - ddmmyy_raw % 100) / 100 - 1;
                    tm.tm_mday = (ddmmyy_raw - (ddmmyy_raw % 10000)) / 10000;
                    const int hhmmss_rawint = hhmmss_raw;
                    tm.tm_sec = hhmmss_rawint % 100;
                    tm.tm_min = ((hhmmss_rawint - (hhmmss_rawint % 100)) % 10000) / 100;
                    tm.tm_hour = (hhmmss_rawint - (hhmmss_rawint % 10000)) / 10000;
                    tm.tm_isdst = 0;

                    std::time_t time = std::mktime(&tm);

                    data->true_unix_time = time;
                    data->update_ts_latlon = capture_time;
                } else {
                }
            } else if (nmea_compare_msg_prefix(this->line_buffer, "TXT")) {
                // $GPTXT,01,01,02,ANTSTATUS=OPEN*2B
            } else {
            }

            this->line_ptr = 0;
            return;
        }
    }
}

GNSS_position GPS_NMEA::get_fix() {
    GNSS_position fix;
    fix.latitude = GPS_data.latitude;
    fix.longitude = GPS_data.longitude;
    fix.altitude = GPS_data.altitude;
    fix.timestamp = GPS_data.update_ts_latlon;
    return fix;
}

uint64_t GPS_NMEA::get_timestamp() {
    if(this->GPS_data.true_unix_time > 0) {
        return this->GPS_data.true_unix_time;
    } else if(this->GALILEO_data.true_unix_time > 0) {
        return this->GALILEO_data.true_unix_time;
    } else {
        return this->OTHER_data.true_unix_time;
    }
}

uint64_t GPS_NMEA::get_message_count() {
    if(this->count_message.count(GNSS_constellation::GPS)) {
        return this->count_message.at(GNSS_constellation::GPS);
    } else {
        return 0;
    }
}

GPS_NMEA::GNSS_raw* GPS_NMEA::get_data_field(GNSS_constellation line_constellation) {
    if (line_constellation == GNSS_constellation::GPS) {
        return &this->GPS_data;
    } else if (line_constellation == GNSS_constellation::GLONASS) {
        return &this->GLONASS_data;
    } else if (line_constellation == GNSS_constellation::GALILEO) {
        return &this->GALILEO_data;
    } else {
        return &this->OTHER_data;
    }
}