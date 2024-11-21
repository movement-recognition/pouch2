#ifndef IFILEIO_H
#define IFILEIO_H

#include <cstdint>
#include <string>
#include <vector>

class IFileIO {
    public:
        IFileIO() {};
        virtual ~IFileIO() {};

        virtual void open_file(std::string filename) = 0;
        virtual void write_line(std::vector<uint8_t> bytes) = 0;
        virtual void close_file(std::string filename) = 0;
};
#endif