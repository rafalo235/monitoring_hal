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

  //!
  //! \brief The CConfiguration class Klasa konfiguracji
  class CConfiguration : public IConfiguration
  {
    friend class CConfigurationFactory;

  protected:

    //! \brief ID koncentratora do wysylania
    uint16_t idConcentrator;

    //! \brief Przerwa pomiedzy wysylanymi danymi
    uint16_t sendingPeriod;

    //! \brief Przerwa pomiedzy sprawdzaniem danych z czujnikow
    uint16_t checkingSensorPeriod;

    //! \brief Konfiguracja dla poszczegolnych sensorow
    std::vector<DSensorConfiguration> sensors;

    //! \brief czy zapisywac dane na SD nawet jesli jest polaczenie z serwerem
    bool saveSDCardIfOnlineEnable;

    //! \brief Wczytuje konfiguracje z pliku
    //! \return true jesli wszystko ok, false jesli nie
    bool saveConfiguration() const;

    //! \brief adres serwera
    std::string serverUrl;

    //! \brief sciezka pliku z konfiguracja
    static const char* fileName;

  public:

    CConfiguration() = default;

    CConfiguration(const CConfiguration&) = delete;
    CConfiguration& operator=(const CConfiguration&) = delete;

    //! \brief Zmienia konfiguracje i zapisuje ja do pliku
    //! \return true jesli wszystko ok, false jesli nie
    virtual bool readConfiguration();

    //! IConfiguration::getCheckingSensorPeriod
    virtual uint16_t getCheckingSensorPeriod() const
    {
      return checkingSensorPeriod;
    }

    //! IConfiguration::getIdConcentrator()
    virtual uint16_t getIdConcentrator() const
    {
      return idConcentrator;
    }

    //! IConfiguration::getSendingPeriod()
    virtual uint16_t getSendingPeriod() const
    {
      return sendingPeriod;
    }

    //! IConfiguration::getSensorConfiguration()
    virtual const std::vector<DSensorConfiguration>& getSensorConfiguration() const
    {
      return sensors;
    }

    //! IConfiguration::getServerUrl()
    virtual const std::string getServerUrl() const
    {
      return serverUrl;
    }

    //! IConfiguration::setCheckingSensorPeriod(const uint16_t)
    virtual bool setCheckingSensorPeriod(const uint16_t checkingSensorPeriod1)
    {
      if (checkingSensorPeriod1 == checkingSensorPeriod){
        return true;
      }
      checkingSensorPeriod = checkingSensorPeriod1;
      return saveConfiguration();
    }

    //! IConfiguration::setSendingPeriod(const uint16_t)
    virtual bool setSendingPeriod(const uint16_t sendingPeriod1)
    {
      if (sendingPeriod1 == sendingPeriod){
        return true;
      }
      sendingPeriod = sendingPeriod1;
      return saveConfiguration();
    }

    //! IConfiguration::setWarningLevel(const int8_t, const CData&)
    virtual bool setWarningLevel(const int8_t idSensor,
                                 const CData& value)
    {
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

    //! IConfiguration::setAlarmLevel(const int8_t, const CData&)
    virtual bool setAlarmLevel(const int8_t idSensor,
                               const CData& value)
    {
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

    //! IConfiguration::setTurnOn(const int8_t, const bool)
    virtual bool setTurnOn(const int8_t idSensor,
                           const bool turnOn)
    {
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

    //! IConfiguration::getSaveSDCardIfOnlineEnable()
    virtual bool getSaveSDCardIfOnlineEnable() const
    {
      return saveSDCardIfOnlineEnable;
    }

    //! IConfiguration::setSaveSDCardIfOnlineEnable(bool)
    virtual bool setSaveSDCardIfOnlineEnable(bool enable)
    {
      saveSDCardIfOnlineEnable = enable;
      return saveConfiguration();
    }

  };
}
#endif // CCONFIGURATION_H
