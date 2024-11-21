#include "pico/stdlib.h"

#include "FatFsSd.h"

#include "SDCardSocket.h"

size_t sd_get_num() {
    return 1;
}

sd_card_t *sd_get_by_num(size_t num) {
    return NULL;
}

SDCardSocket::SDCardSocket() {
    this->spi_settings.hw_inst = spi0;
    this->spi_settings.sck_gpio = 18;
    this->spi_settings.mosi_gpio = 19;
    this->spi_settings.miso_gpio = 16;
    this->spi_settings.baud_rate = 125 * 1000 * 1000 / 4; // 31,25 MHz
    this->spi_if_settings.spi = &this->spi_settings;
    this->spi_if_settings.ss_gpio = 17;

    this->init_card();
}

SDCardSocket::SDCardSocket(spi_t* spi_settings_, uint8_t cs_gpio_) {
    this->spi_settings = *spi_settings_;
    this->spi_if_settings.spi = &this->spi_settings;
    this->spi_if_settings.ss_gpio = cs_gpio_;

    this->init_card();
}

void SDCardSocket::init_card() {
    this->spi_if_settings.spi = &this->spi_settings;
    this->sd_card.type = SD_IF_SPI;
    this->sd_card.spi_if_p = &spi_if_settings;
    this->sd_card.use_card_detect = false;

    // this->card_p = new FatFsNs::SdCard(&this->sd_card);
    FatFsNs::SdCard* card_p(FatFsNs::FatFs::add_sd_card(&sd_card));

    sd_init_driver(); 
}

void SDCardSocket::open_file(std::string filename) {

}

void SDCardSocket::write_line(std::vector<uint8_t> bytes) {
    FRESULT fr = this->card_p->mount();
    FatFsNs::File file;
    char const* const filename = "filename.txt";
    fr = file.open(filename, FA_OPEN_APPEND | FA_WRITE);
    std::string foo = "foo";
    char const* const str = "Hello, world!\n";
    file.printf(str);
    fr = file.close();
    fr = this->card_p->unmount();   
}

void SDCardSocket::close_file(std::string filename) {

};