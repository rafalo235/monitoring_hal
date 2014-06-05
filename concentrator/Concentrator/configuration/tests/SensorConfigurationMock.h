#ifndef SENSORCONFIGURATIONMOCK_H
#define SENSORCONFIGURATIONMOCK_H

#include "configuration/core/SensorConfiguration.h"

namespace NEngine{

class CSensorConfigurationMock : public CSensorConfiguration
{

public:
    CSensorConfigurationMock(uint8_t& idSensor1,
                         bool& turnOn1,
                         CData& warningLvl1,
                         CData& alarmLvl1) :
                    CSensorConfiguration(idSensor1, turnOn1, warningLvl1, alarmLvl1)
    {

    }
};

}
#endif // SENSORCONFIGURATIONMOCK_H
