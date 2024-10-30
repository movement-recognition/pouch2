#ifndef II2C_H
#define II2C_H

class II2C {
    public:
        II2C() {};
        virtual ~II2C() {};

        virtual void print_scan() = 0;

};
#endif