#ifndef SDCARDSOCKET_H
#define SDCARDSOCKET_H

#include <cstdint>
#include <vector>
#include <map>
#include <string>

#include "tf_card.h"
#include "ff.h"

#include "IFileIO.h"

class SDCardSocket: public IFileIO {
    public:
        SDCardSocket(bool foo);
        void mount_card();
        void open_file(std::string filename);
        void write_line(std::string data);
        void write_line(char* data);
        void flush();
        void close_file();
        size_t get_file_size();
    private:
        char mount_name;
        
        FATFS fs;
        FIL fil;
        FRESULT fr;     /* FatFs return code */
        UINT br;
        UINT bw;
};
#endif