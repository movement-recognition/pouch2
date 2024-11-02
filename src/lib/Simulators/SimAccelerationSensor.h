#ifndef SIM_ACCELERATION_SENSOR_H
#define SIM_ACCELERATION_SENSOR_H

#include "IAccelerationSensor.h"


class SimAccelerationSensor: public IAccelerationSensor {
    public: 
        SimAccelerationSensor();

        acceleration_struct get_imu_data();
    
    private:
};

#endif
