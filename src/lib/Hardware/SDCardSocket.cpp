#include "pico/stdlib.h"

#include "SDCardSocket.h"


SDCardSocket::SDCardSocket(bool foo) {
    // this->spi_settings.sck_gpio = 18;
    // this->spi_settings.mosi_gpio = 19;
    // this->spi_settings.miso_gpio = 16;
    // this->spi_settings.baud_rate = 125 * 1000 * 1000 / 4; // 31,25 MHz
    // this->spi_if_settings.ss_gpio = 17;

    this->init_card();
}

void SDCardSocket::init_card() {

}

void SDCardSocket::open_file(std::string filename) {

}

void SDCardSocket::write_line(std::string) {

}

void SDCardSocket::close_file(std::string filename) {

};