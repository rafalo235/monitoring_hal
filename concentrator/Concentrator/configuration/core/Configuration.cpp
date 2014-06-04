#include <algorithm>
#include <vector>
#include <fstream>
#include <cstdint>

#include "configuration/core/Configuration.h"
#include "configuration/core/SensorConfiguration.h"

namespace NEngine {

  const char* CConfiguration::fileName = "configuration.conf";

  bool CConfiguration::readConfiguration() {
    std::ifstream file(fileName);
    if (!file.is_open()){
      return false;
    }
    //TODO: sprawdzanie czy plik jest poprawny
    decltype(SProtocol::idConcentrator) idConcentrator;
    uint16_t sendingPeriod;
    uint16_t checkingSensorPeriod;
    uint8_t sensorsAmount;
    bool saveSDCardIfOnlineEnable;
    file.read(reinterpret_cast<char*>(&idConcentrator), sizeof(idConcentrator));
    file.read(reinterpret_cast<char*>(&sendingPeriod), sizeof(sendingPeriod));
    file.read(reinterpret_cast<char*>(&checkingSensorPeriod), sizeof(checkingSensorPeriod));
    file.read(reinterpret_cast<char*>(&saveSDCardIfOnlineEnable), sizeof(saveSDCardIfOnlineEnable));
    file.read(reinterpret_cast<char*>(&sensorsAmount), sizeof(sensorsAmount));

    std::vector<DSensorConfiguration> sensors;
    decltype(SSensorData::idSensor) id;
    bool turnOn;
    SData warning;
    SData alarm;

    for(int i = 0; i < sensorsAmount; ++i){
      file.read(reinterpret_cast<char*>(&id), sizeof(id));
      file.read(reinterpret_cast<char*>(&turnOn), sizeof(turnOn));
      file.read(reinterpret_cast<char*>(&warning), sizeof(warning));
      file.read(reinterpret_cast<char*>(&alarm), sizeof(alarm));

      DSensorConfiguration buf(new CSensorConfiguration(id, turnOn, warning, alarm));
      sensors.push_back(buf);
    }
    file.close();
    this->idConcentrator = idConcentrator;
    this->sendingPeriod = sendingPeriod;
    this->checkingSensorPeriod = checkingSensorPeriod;
    this->sendingPeriod = sendingPeriod;
    this->sensors = sensors;
    this->saveSDCardIfOnlineEnable = saveSDCardIfOnlineEnable;
    return true;
  }

  bool CConfiguration::saveConfiguration()const{

    std::ofstream file(fileName);
    if (!file.is_open()){
      return false;
    }
// TODO: sprawdzanie czy plik jest poprawny - CRC
    const uint8_t sensorsAmount = static_cast<uint8_t>(sensors.size());

    file.write(reinterpret_cast<const char*>(&idConcentrator), sizeof(idConcentrator));
    file.write(reinterpret_cast<const char*>(&sendingPeriod), sizeof(sendingPeriod));
    file.write(reinterpret_cast<const char*>(&checkingSensorPeriod), sizeof(checkingSensorPeriod));
    file.write(reinterpret_cast<const char*>(&saveSDCardIfOnlineEnable), sizeof(saveSDCardIfOnlineEnable));


    file.write(reinterpret_cast<const char*>(&sensorsAmount), sizeof(sensorsAmount));

    std::for_each(sensors.begin(), sensors.end(),
                  [&](const DSensorConfiguration& c)
                    {
                        decltype(SSensorData::idSensor) id = c->getSensorId();
                        bool turnOn = c->isTurnOn();
                        const SData warning = c->getWarnigLvl();
                        const SData alarm = c->getAlarmLvl();
                        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
                        file.write(reinterpret_cast<const char*>(&turnOn), sizeof(turnOn));
                        file.write(reinterpret_cast<const char*>(&warning), sizeof(warning));
                        file.write(reinterpret_cast<const char*>(&alarm), sizeof(alarm));

                    });

    file.close();
    return true;
  }
}
