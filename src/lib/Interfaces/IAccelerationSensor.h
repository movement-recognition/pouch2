#ifndef ILINESENSOR_H
#define ILINESENSOR_H


class IAccelerationSensor {
    public:
        IAccelerationSensor() {};

        virtual ~IAccelerationSensor() {};


        virtual void initialize() {};

        virtual void getAccelerationTriple() = 0;

};

#endif