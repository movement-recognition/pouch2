#ifndef ACCELERATION_MPU6050_SENSOR_H
#define ACCELERATION_MPU6050_SENSOR_H

#include "IAccelerationSensor.h"

class MPU6050_Sensor: public IAccelerationSensor {
    private:

    public: 
        void initialize();

        void getAccelerationTriple();

};

#endif
