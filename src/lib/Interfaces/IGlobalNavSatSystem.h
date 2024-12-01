#ifndef IGLOBALNAVSATSYSTEM_H
#define IGLOBALNAVSATSYSTEM_H

#include <cstdint>

struct GNSS_position {
    uint64_t timestamp;
    int32_t latitude;
    int32_t longitude;
    int32_t altitude;
};

class IGlobalNavSatSystem {
    public:
        IGlobalNavSatSystem() {};
        virtual ~IGlobalNavSatSystem() {};
        virtual void poll();
        virtual uint64_t get_timestamp() = 0;
        virtual GNSS_position get_fix() = 0;
};


#endif