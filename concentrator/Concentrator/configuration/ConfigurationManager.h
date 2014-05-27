#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "configuration/Configuration.h"
#include <memory>

namespace NEngine{

  class CConfigurationManager;
  typedef std::shared_ptr<CConfigurationManager>  DConfigurationManager;

  //! \brief Klasa odczytuje z pliku konfiguracje. Konfiguracja musi byc zapisywalna.
  class CConfigurationManager
  {
    static std::shared_ptr<CConfigurationManager> manager;

    CConfiguration conf;
    CConfigurationManager() = default;
    CConfigurationManager(const CConfigurationManager&) = delete;
    CConfigurationManager& operator=(const CConfigurationManager&) = delete;

  public:

    //! \return zwraca smart pointer do instacji manager'a - singleton
    static std::shared_ptr<CConfigurationManager> getInstance(){
      if (manager.use_count() == 0){
        manager.reset(new CConfigurationManager());
      }
      return manager;
    }

    //! \brief Wczytuje konfiguracje z pliku
    //! \return true jesli wszystko ok, false jesli nie
    bool readConfiguration(){
      std::ifstream file("configuration.conf");
      if (!file.is_open()){
        return false;
      }
      decltype(SProtocol::idConcentrator) idConcentrator;
      uint16_t sendingPeriod;
      uint16_t checkingSensorPeriod;
      uint8_t sensorsAmount;

      file.read(reinterpret_cast<char*>(&idConcentrator), sizeof(idConcentrator));
      file.read(reinterpret_cast<char*>(&sendingPeriod), sizeof(sendingPeriod));
      file.read(reinterpret_cast<char*>(&checkingSensorPeriod), sizeof(checkingSensorPeriod));
      file.read(reinterpret_cast<char*>(&sensorsAmount), sizeof(sensorsAmount));

      std::vector<CSensorConfiguration> sensors;
      for(int i = 0; i < sensorsAmount; ++i){
        CSensorConfiguration conf;
        file.read(reinterpret_cast<char*>(&conf), sizeof(conf));
        sensors.push_back(conf);
      }

      CConfiguration conf2(idConcentrator, sendingPeriod, sensors, checkingSensorPeriod);
      conf = conf2;
      file.close();
      return true;
    }


    //! \brief Zmienia konfiguracje i zapisuje ja do pliku
    //! \return true jesli wszystko ok, false jesli nie
    bool changeConfiguration(const CConfiguration& configuration){
      conf = configuration;
      std::ofstream file("configuration.conf");
      if (!file.is_open()){
        return false;
      }
      decltype(SProtocol::idConcentrator) idConcentrator = configuration.getIdConcentrator();
      uint16_t sendingPeriod = configuration.getSendingPeriod();
      uint16_t checkingSensorPeriod = configuration.getCheckingSensorPeriod();
      const std::vector<CSensorConfiguration>* sensors = configuration.getSensorConfiguration();
      uint8_t sensorsAmount = static_cast<uint8_t>(sensors->size());

      file.write(reinterpret_cast<char*>(&idConcentrator), sizeof(idConcentrator));
      file.write(reinterpret_cast<char*>(&sendingPeriod), sizeof(sendingPeriod));
      file.write(reinterpret_cast<char*>(&checkingSensorPeriod), sizeof(checkingSensorPeriod));
      file.write(reinterpret_cast<char*>(&sensorsAmount), sizeof(sensorsAmount));

      std::for_each(sensors->begin(), sensors->end(),
                    [&](const CSensorConfiguration& c){file.write(reinterpret_cast<const char*>(&c), sizeof(c));});
      file.close();
      return true;
    }

    const CConfiguration* getConfiguration(){
      return &conf;
    }

  };
}
#endif // CONFIGURATIONMANAGER_H
