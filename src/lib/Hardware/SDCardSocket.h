#ifndef SDCARDSOCKET_H
#define SDCARDSOCKET_H

#include <cstdint>
#include <vector>
#include <string>


#include "IFileIO.h"

class SDCardSocket: public IFileIO {
    public:
        SDCardSocket(bool foo);
        void open_file(std::string filename);
        void write_line(std::string string);
        void close_file(std::string filename);
    private:
        char mount_name;
        void init_card();
};
#endif