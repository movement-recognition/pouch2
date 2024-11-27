#include <cstring>
#include "pico/stdlib.h"

#include "tf_card.h"
#include "ff.h"
#include "SDCardSocket.h"


SDCardSocket::SDCardSocket(bool foo) {
    // this->spi_settings.sck_gpio = 18;
    // this->spi_settings.mosi_gpio = 19;
    // this->spi_settings.miso_gpio = 16;
    // this->spi_settings.baud_rate = 125 * 1000 * 1000 / 4; // 31,25 MHz
    // this->spi_if_settings.ss_gpio = 17;
    for(int pin = 16; pin <= 19; pin++) {
        gpio_init(pin);
        // gpio_pull_up(pin);
        gpio_set_function(pin, GPIO_FUNC_SPI);
    }
    
    pico_fatfs_spi_config_t config = {
        spi0,
        CLK_SLOW_DEFAULT,
        CLK_FAST_DEFAULT,
        16, // PIN_SPI0_MISO_DEFAULT,
        17, // PIN_SPI0_CS_DEFAULT,
        18, // PIN_SPI0_SCK_DEFAULT,
        19, // PIN_SPI0_MOSI_DEFAULT,
        false  // use internal pullup
    };
    pico_fatfs_set_config(&config);
}

void SDCardSocket::mount_card() {
    for (int i = 0; i < 5; i++) {
        fr = f_mount(&fs, "", 1);
        if (fr == FR_OK) { break; }
        printf("mount error %d -> retry %d\n", fr, i);
        pico_fatfs_reboot_spi();
    }
    if (fr != FR_OK) {
        printf("mount error %d\n", fr);
    }
    printf("mount ok\n");

    switch (fs.fs_type) {
        case FS_FAT12:
            printf("Type is FAT12\n");
            break;
        case FS_FAT16:
            printf("Type is FAT16\n");
            break;
        case FS_FAT32:
            printf("Type is FAT32\n");
            break;
        case FS_EXFAT:
            printf("Type is EXFAT\n");
            break;
        default:
            printf("Type is unknown\n");
            break;
    }
    printf("Card size: %7.2f GB (GB = 1E9 bytes)\n\n", fs.csize * fs.n_fatent * 512E-9);
}

void SDCardSocket::open_file(std::string filename) {
    fr = f_open(&fil, filename.c_str(), FA_READ | FA_WRITE | FA_OPEN_APPEND);
    FSIZE_t size = f_size(&fil);
    f_lseek(&fil, size);
}

void SDCardSocket::write_line(std::string data) {
    char* buf = (char*) data.c_str();
    size_t buf_size = std::strlen(buf);
    fr = f_write(&fil, buf, buf_size, &bw);
    if (fr != FR_OK || bw != buf_size) {
        printf("write failed %d %d\n", fr, bw);
    }
}

void SDCardSocket::write_line(char* data) {
    size_t buf_size = std::strlen(data);
    fr = f_write(&fil, data, buf_size, &bw);
    if (fr != FR_OK || bw != buf_size) {
        printf("write failed %d %d\n", fr, bw);
    }
}

void SDCardSocket::flush() {
    f_sync(&fil);
}

size_t SDCardSocket::get_file_size() {
    return f_size(&fil);
}

void SDCardSocket::close_file() {
    f_close(&fil);
};