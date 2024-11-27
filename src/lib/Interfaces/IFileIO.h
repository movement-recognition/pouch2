#ifndef IFILEIO_H
#define IFILEIO_H

#include <cstdint>
#include <string>
#include <vector>

class IFileIO {
    public:
        IFileIO() {};
        virtual ~IFileIO() {};
        virtual void mount_card() = 0;
        virtual void open_file(std::string filename) = 0;
        virtual void write_line(std::string data) = 0;
        virtual void flush() = 0;
        virtual void close_file() = 0;
        virtual size_t get_file_size();
};
#endif