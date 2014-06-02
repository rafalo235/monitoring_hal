#include <algorithm>
#include <vector>
#include <fstream>
#include <cstdint>

#include "configuration/core/Configuration.h"
#include "configuration/core/SensorConfiguration.h"

namespace NEngine {

  bool CConfiguration::readConfiguration() {
    std::ifstream file("configuration.conf");
    if (!file.is_open()){
      return false;
    }
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
    CSensorConfiguration conf;
    for(int i = 0; i < sensorsAmount; ++i){


      file.read(reinterpret_cast<char*>(&conf), sizeof(conf));
      CSensorConfiguration* a = new CSensorConfiguration(conf);
      DSensorConfiguration buf(a);
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

    std::ofstream file("configuration.conf");
    if (!file.is_open()){
      return false;
    }

    const uint8_t sensorsAmount = static_cast<uint8_t>(sensors.size());

    file.write(reinterpret_cast<const char*>(&idConcentrator), sizeof(idConcentrator));
    file.write(reinterpret_cast<const char*>(&sendingPeriod), sizeof(sendingPeriod));
    file.write(reinterpret_cast<const char*>(&checkingSensorPeriod), sizeof(checkingSensorPeriod));
    file.write(reinterpret_cast<const char*>(&saveSDCardIfOnlineEnable), sizeof(saveSDCardIfOnlineEnable));


    file.write(reinterpret_cast<const char*>(&sensorsAmount), sizeof(sensorsAmount));

    std::for_each(sensors.begin(), sensors.end(),
                  [&](const DSensorConfiguration& c){file.write(reinterpret_cast<const char*>(&(*c)), sizeof(*c));});
    file.close();
    return true;
  }
}
