#include "ConfigurationMock.h"
#include "configuration/core/SensorConfiguration.h"
#include "configuration/tests/SensorConfigurationMock.h"
#include "util/Logger.h"

namespace NEngine{

  CConfigurationMock::CConfigurationMock()
  {
    LOG_DEBUG("CConfigurationMock - constructor.");
    idConcentrator = 1;
    sendingPeriod = 10;
    checkingSensorPeriod = 2;
    saveSDCardIfOnlineEnable = false;
    dataPath = "sdcard/";

    int warningValue = 10;
    int alarmValue = 12;
    CData warning(EValueType::INT_32, &warningValue);

    CData alarm(EValueType::INT_32, &alarmValue);

    for (int8_t i = 0; i < 3; ++i)
    {

      uint8_t idSensor1 = i;
      bool turnOn = true;

      DSensorConfiguration s(
                  new CSensorConfigurationMock(idSensor1, turnOn, warning, alarm));
      this->sensors.push_back(s);

    }
    serverUrl = "http://localhost:8080/HallMonitorServer/rest/concentrator/post";
    saveConfiguration();
  }
}
