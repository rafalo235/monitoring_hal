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
    uint16_t idConcentrator;
    uint16_t sendingPeriod;
    uint16_t checkingSensorPeriod;
    uint8_t sensorsAmount;
    bool saveSDCardIfOnlineEnable;

    uint8_t serverUrlLength;

    file.read(reinterpret_cast<char*>(&idConcentrator), sizeof(idConcentrator));
    file.read(reinterpret_cast<char*>(&sendingPeriod), sizeof(sendingPeriod));
    file.read(reinterpret_cast<char*>(&checkingSensorPeriod), sizeof(checkingSensorPeriod));
    file.read(reinterpret_cast<char*>(&saveSDCardIfOnlineEnable), sizeof(saveSDCardIfOnlineEnable));
    file.read(reinterpret_cast<char*>(&serverUrlLength), sizeof(serverUrlLength));
    std::string serverUrl(serverUrlLength, '\0');

    file.read(&serverUrl[0], serverUrlLength);
    file.read(reinterpret_cast<char*>(&sensorsAmount), sizeof(sensorsAmount));

    std::vector<DSensorConfiguration> sensors;
    uint8_t idSensor;
    bool turnOn;
    CData warning;
    CData alarm;
    uint8_t ipv4Length;

    for(int i = 0; i < sensorsAmount; ++i){
      int port;

      file.read(reinterpret_cast<char*>(&idSensor), sizeof(idSensor));
      file.read(reinterpret_cast<char*>(&turnOn), sizeof(turnOn));
      file.read(reinterpret_cast<char*>(&warning), sizeof(warning));
      file.read(reinterpret_cast<char*>(&alarm), sizeof(alarm));
      file.read(reinterpret_cast<char*>(&ipv4Length), sizeof(ipv4Length));
      std::string ipv4(ipv4Length, '\0');
      file.read(&ipv4[0], ipv4Length);
      file.read(reinterpret_cast<char*>(&port), sizeof(port));

      DSensorConfiguration buf(new CSensorConfiguration(idSensor, turnOn, warning, alarm, ipv4, port));
      sensors.push_back(buf);
    }
    file.close();
    this->idConcentrator = idConcentrator;
    this->sendingPeriod = sendingPeriod;
    this->checkingSensorPeriod = checkingSensorPeriod;
    this->sendingPeriod = sendingPeriod;
    this->sensors = sensors;
    this->saveSDCardIfOnlineEnable = saveSDCardIfOnlineEnable;
    this->serverUrl = serverUrl;
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
    uint8_t serverUrlLength = static_cast<uint8_t>(serverUrl.length());
    file.write(reinterpret_cast<const char*>(&serverUrlLength), sizeof(serverUrlLength));
    const char* serverUrlBuf = serverUrl.c_str();

    file.write(serverUrlBuf, serverUrlLength);
    file.write(reinterpret_cast<const char*>(&sensorsAmount), sizeof(sensorsAmount));

    std::for_each(sensors.begin(), sensors.end(),
                  [&](const DSensorConfiguration& c)
                    {
                        uint8_t idSensor = c->getSensorId();
                        bool turnOn = c->isTurnOn();
                        const CData warning = c->getWarnigLvl();
                        const CData alarm = c->getAlarmLvl();
                        std::string ipv4 = c->getIpv4Address();
                        uint8_t ipv4Length = static_cast<uint8_t>(ipv4.length());
                        int port = c->getPort();
                        file.write(reinterpret_cast<const char*>(&idSensor), sizeof(idSensor));
                        file.write(reinterpret_cast<const char*>(&turnOn), sizeof(turnOn));
                        file.write(reinterpret_cast<const char*>(&warning), sizeof(warning));
                        file.write(reinterpret_cast<const char*>(&alarm), sizeof(alarm));
                        file.write(reinterpret_cast<const char*>(&ipv4Length), sizeof(ipv4Length));
                        file.write(ipv4.c_str(), ipv4Length);
                        file.write(reinterpret_cast<const char*>(&port), sizeof(port));
                    });

    file.close();
    return true;
  }
}
