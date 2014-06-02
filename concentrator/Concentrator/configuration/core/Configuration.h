#ifndef CCONFIGURATION_H
#define CCONFIGURATION_H

#include <fstream>
#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>

#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/IConfiguration.h"
#include "configuration/core/SensorConfiguration.h"

namespace NEngine {

  using namespace NProtocol;
  class CConfigurationFactory;

  class CConfiguration : public IConfiguration
  {
    friend class CConfigurationFactory;

    //! \brief ID koncentratora do wysylania
    decltype(SProtocol::idConcentrator) idConcentrator;

    //! \brief Przerwa pomiedzy wysylanymi danymi
    uint16_t sendingPeriod;

    //! \brief Przerwa pomiedzy sprawdzaniem danych z czujnikow
    uint16_t checkingSensorPeriod;

    //! \brief Konfiguracja dla poszczegolnych sensorow
    std::vector<DSensorConfiguration> sensors;

    bool saveSDCardIfOnlineEnable;

    //! \brief Wczytuje konfiguracje z pliku
    //! \return true jesli wszystko ok, false jesli nie
    bool saveConfiguration() const;

  public:

    CConfiguration()
    {
    }

    CConfiguration(const CConfiguration&) = delete;
    CConfiguration& operator=(const CConfiguration&) = delete;

    //! \brief Zmienia konfiguracje i zapisuje ja do pliku
    //! \return true jesli wszystko ok, false jesli nie
    virtual bool readConfiguration();

    //! \return Zwraca przerwe pomiedzy sprawdzaniem danych z czujnikow
    virtual uint16_t getCheckingSensorPeriod() const{
      return checkingSensorPeriod;
    }

    //! \return Zwraca id koncentratora odczytane z pliku konfiguracyjnego
    virtual decltype(SProtocol::idConcentrator) getIdConcentrator() const{
      return idConcentrator;
    }

    //! \return Zwraca przerwe pomiedzy wysylanymi danymi
    virtual uint16_t getSendingPeriod() const{
      return sendingPeriod;
    }

    //! \return Zwraca konfiguracje czujnikow
    virtual const std::vector<DSensorConfiguration>* getSensorConfiguration()const{
      return &sensors;
    }

    virtual bool setCheckingSensorPeriod(const uint16_t checkingSensorPeriod1){
      if (checkingSensorPeriod1 == checkingSensorPeriod){
        return true;
      }
      checkingSensorPeriod = checkingSensorPeriod1;
      return saveConfiguration();
    }

    virtual bool setSendingPeriod(const uint16_t sendingPeriod1){
      if (sendingPeriod1 == sendingPeriod){
        return true;
      }
      sendingPeriod = sendingPeriod1;
      return saveConfiguration();
    }

    virtual bool setWarningLevel(const decltype(SSensorData::idSensor) idSensor, const SData& value){
      std::vector<DSensorConfiguration>::iterator it = std::find_if(
                                                         sensors.begin(), sensors.end(),
                                                         [&](DSensorConfiguration& s){return s->getSensorId() == idSensor;});

      if (it == sensors.end()){
        return false;
      }
      std::shared_ptr<CSensorConfiguration> s = std::dynamic_pointer_cast<CSensorConfiguration>(*it);
      s->setWarningLvl(value);
      return saveConfiguration();
    }

    virtual bool setAlarmLevel(const decltype(SSensorData::idSensor) idSensor, const SData& value){
      std::vector<DSensorConfiguration>::iterator it = std::find_if(
                                                         sensors.begin(), sensors.end(),
                                                         [&](DSensorConfiguration& s){return s->getSensorId() == idSensor;});

      if (it == sensors.end()){
        return false;
      }
      std::shared_ptr<CSensorConfiguration> s = std::dynamic_pointer_cast<CSensorConfiguration>(*it);
      s->setAlarmLvl(value);
      return saveConfiguration();
    }

    virtual bool setTurnOn(const decltype(SSensorData::idSensor) idSensor, const bool turnOn){
      std::vector<DSensorConfiguration>::iterator it = std::find_if(
                                                         sensors.begin(), sensors.end(),
                                                         [&](DSensorConfiguration& s){return s->getSensorId() == idSensor;});

      if (it == sensors.end()){
        return false;
      }
      std::shared_ptr<CSensorConfiguration> s = std::dynamic_pointer_cast<CSensorConfiguration>(*it);
      s->setTurnOn(turnOn);
      return saveConfiguration();
    }

    //! \return true, jesli dane maja byc ciagle zapisywane na karte SD nawet jeśli polaczenie z serwerem jest OK
    virtual bool getSaveSDCardIfOnlineEnable() const
    {
      return saveSDCardIfOnlineEnable;
    }

    virtual bool setSaveSDCardIfOnlineEnable(bool enable)
    {
      saveSDCardIfOnlineEnable = enable;
      return saveConfiguration();
    }

  };
}
#endif // CCONFIGURATION_H
