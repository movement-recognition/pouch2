#ifndef SDCARDSOCKET_H
#define SDCARDSOCKET_H

#include <cstdint>
#include <vector>
#include <string>

// #include "hw_config.h"
#include "FatFsSd.h"

#include "IFileIO.h"

class SDCardSocket: public IFileIO {
    public:
        SDCardSocket();
        SDCardSocket(spi_t* spi_settings_, uint8_t cs_gpio_);
        void open_file(std::string filename);
        void write_line(std::vector<uint8_t> bytes);
        void close_file(std::string filename);
    private:
        char mount_name;
        spi_t spi_settings;
        sd_spi_if_t spi_if_settings;

        sd_card_t sd_card;
        void init_card();

        FatFsNs::SdCard* card_p;
};
#endif