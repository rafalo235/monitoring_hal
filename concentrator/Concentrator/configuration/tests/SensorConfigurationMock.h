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
                         CData& alarmLvl1,
                         std::string& ipv4Address1,
                         int& port1) :
                    CSensorConfiguration(idSensor1, turnOn1, warningLvl1, alarmLvl1, ipv4Address1, port1)
    {

    }
};

}
#endif // SENSORCONFIGURATIONMOCK_H
