#ifndef SDCARDSOCKET_H
#define SDCARDSOCKET_H

#include <cstdint>
#include <vector>
#include <string>

#include "tf_card.h"
#include "ff.h"

#include "IFileIO.h"

class SDCardSocket: public IFileIO {
    public:
        SDCardSocket(bool foo);
        void mount_card();
        void open_file(std::string filename);
        void write_line(std::string string);
        void close_file(std::string filename);
    private:
        char mount_name;
        
        FATFS fs;
        FIL fil;
        FRESULT fr;     /* FatFs return code */
        UINT br;
        UINT bw;
};
#endif