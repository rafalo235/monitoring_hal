#include "ConfigurationMock.h"
#include "configuration/core/SensorConfiguration.h"

namespace NEngine{
  CConfigurationMock::CConfigurationMock()
  {

    idConcentrator = 1;
    sendingPeriod = 10;
    checkingSensorPeriod = 2;
    saveSDCardIfOnlineEnable = false;
    SData warning;
    warning.type = EValueType::INT_32;
    warning.value.vInt32 = 10;

    SData alarm;
    alarm.type = EValueType::INT_32;
    alarm.value.vInt32 = 12;
    for (decltype(SSensorData::idSensor) i = 0; i < 3; ++i)
    {

      DSensorConfiguration s(new CSensorConfiguration());

      std::shared_ptr<CSensorConfiguration> s1
          = std::dynamic_pointer_cast<CSensorConfiguration>(s);
      s1->idSensor = i;
      s1->turnOn = true;
      s1->warningLvl = warning;
      s1->alarmLvl= alarm;
      this->sensors.push_back(s);

    }
    saveConfiguration();
  }
}
