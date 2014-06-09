#include <algorithm>
#include <vector>
#include <fstream>
#include <cstdint>

#include "configuration/core/Configuration.h"
#include "configuration/core/SensorConfiguration.h"

namespace NEngine {

  const std::string CConfiguration::fileName("configuration.conf");

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

    // odczyt konfiguracji koncentratora
    file.read(reinterpret_cast<char*>(&idConcentrator), sizeof(idConcentrator));
    file.read(reinterpret_cast<char*>(&sendingPeriod), sizeof(sendingPeriod));
    file.read(reinterpret_cast<char*>(&checkingSensorPeriod), sizeof(checkingSensorPeriod));
    file.read(reinterpret_cast<char*>(&saveSDCardIfOnlineEnable), sizeof(saveSDCardIfOnlineEnable));

    std::string serverUrl = readString(file);
    std::string dataPath = readString(file);

    // odczyt konfiguracji czujnikow
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
    // dopiero jak wszystko pojdzie ok, to zastap istniejaca konfiguracje
    this->idConcentrator = idConcentrator;
    this->sendingPeriod = sendingPeriod;
    this->checkingSensorPeriod = checkingSensorPeriod;
    this->sendingPeriod = sendingPeriod;
    this->sensors = sensors;
    this->saveSDCardIfOnlineEnable = saveSDCardIfOnlineEnable;
    this->serverUrl = serverUrl;
    this->dataPath = dataPath;
    return true;
  }

  bool CConfiguration::saveConfiguration()const{

    std::ofstream file(fileName);
    if (!file.is_open()){
      return false;
    }
// TODO: sprawdzanie czy plik jest poprawny - CRC

    // zapis konfiguracji koncentratora
    file.write(reinterpret_cast<const char*>(&idConcentrator), sizeof(idConcentrator));
    file.write(reinterpret_cast<const char*>(&sendingPeriod), sizeof(sendingPeriod));
    file.write(reinterpret_cast<const char*>(&checkingSensorPeriod), sizeof(checkingSensorPeriod));
    file.write(reinterpret_cast<const char*>(&saveSDCardIfOnlineEnable), sizeof(saveSDCardIfOnlineEnable));

    writeString(file, serverUrl);
    writeString(file, dataPath);

    // zapis konfigurakcji czujnikow
    const uint8_t sensorsAmount = static_cast<uint8_t>(sensors.size());
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
    file.flush();
    file.close();
    return true;
  }

  template<typename T>
  bool CConfiguration::changeValue(T& currentValue, const T& newValue)
  {
    // jesli ustawiana jest taka sama wartosc to nic nie zmieniaj
    if (currentValue == newValue){
      return true;
    }
    mutex.lock();
    T odlValue = currentValue;
    currentValue = newValue;
    const bool result = saveConfiguration();
    if (!result)
    {
      // jesli zapis sie nie powiodl, przywroc stare ustawienia
      currentValue = odlValue;
    }
    mutex.unlock();
    return result;
  }


  template<typename T>
  bool CConfiguration::setSensorValue(const int8_t idSensor, const T (CSensorConfiguration::*getter)() const,
                      void (CSensorConfiguration::*setter)(const T&), const T& newValue)
  {
    std::vector<DSensorConfiguration>::iterator it = std::find_if(
                                                       sensors.begin(), sensors.end(),
                                                       [&](DSensorConfiguration& s){return s->getSensorId() == idSensor;});
    if (it == sensors.end())
    {
      // nie znaleziono czujnika
      return false;
    }
    std::shared_ptr<CSensorConfiguration> s = std::dynamic_pointer_cast<CSensorConfiguration>(*it);
    T currentValue = (*s.*getter)();
    mutex.lock();
    T oldValue = currentValue;
    (*s.*setter)(newValue);

    bool result = saveConfiguration();
    if (!result)
    {
      // nie udal sie zapis - przywroc stare wartosci
      (*s.*setter)(oldValue);
    }

    mutex.unlock();
    return result;
  }

  template<typename T>
  bool CConfiguration::setSensorValue(const int8_t idSensor, T (CSensorConfiguration::*getter)() const,
                      void (CSensorConfiguration::*setter)(const T), const T& newValue)
  {
    std::vector<DSensorConfiguration>::iterator it = std::find_if(
                                                       sensors.begin(), sensors.end(),
                                                       [&](DSensorConfiguration& s){return s->getSensorId() == idSensor;});
    if (it == sensors.end())
    {
      // nie znaleziono czujnika
      return false;
    }
    std::shared_ptr<CSensorConfiguration> s = std::dynamic_pointer_cast<CSensorConfiguration>(*it);
    T currentValue = (*s.*getter)();
    mutex.lock();
    T oldValue = currentValue;
    (*s.*setter)(newValue);

    bool result = saveConfiguration();
    if (!result)
    {
      // nie udal sie zapis - przywroc stare wartosci
      (*s.*setter)(oldValue);
    }

    mutex.unlock();
    return result;
  }
}
