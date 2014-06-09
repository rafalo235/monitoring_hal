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

    std::string sensorAddr = "127.0.0.1";
    int port = 1502;

    for (int8_t i = 0; i < 1; ++i)
    {

      uint8_t idSensor1 = i;
      bool turnOn = true;

      DSensorConfiguration s(
                  new CSensorConfigurationMock(idSensor1, turnOn, warning, alarm, sensorAddr, port));
      this->sensors.push_back(s);
      port = port + 1;

    }
    serverUrl = "http://localhost:8080/HallMonitorServer/rest/concentrator/post";
    saveConfiguration();
  }
}
