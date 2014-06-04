#ifndef ICONFIGURATION_H
#define ICONFIGURATION_H

#include <memory>
#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/ISensorConfiguration.h"

namespace NEngine{
  typedef std::shared_ptr<ISensorConfiguration> DSensorConfiguration;

  class IConfiguration
  {
  public:
    //! \brief Zmienia konfiguracje i zapisuje ja do pliku
    //! \return true jesli wszystko ok, false jesli nie
    virtual bool readConfiguration() = 0;

    //! \return Zwraca przerwe pomiedzy sprawdzaniem danych z czujnikow
    virtual uint16_t getCheckingSensorPeriod() const = 0;

    //! \return Zwraca id koncentratora odczytane z pliku konfiguracyjnego
    virtual decltype(SProtocol::idConcentrator) getIdConcentrator() const = 0;

    //! \return Zwraca przerwe pomiedzy wysylanymi danymi
    virtual uint16_t getSendingPeriod() const = 0;

    //! \return Zwraca konfiguracje czujnikow
    virtual const std::vector<DSensorConfiguration>& getSensorConfiguration()const =0;

    //! \return true, jesli dane maja byc ciagle zapisywane na karte SD nawet jeśli polaczenie z serwerem jest OK
    virtual bool getSaveSDCardIfOnlineEnable() const = 0;

    virtual bool setCheckingSensorPeriod(const uint16_t checkingSensorPeriod1) = 0;

    virtual bool setSendingPeriod(const uint16_t sendingPeriod1) = 0;

    virtual bool setWarningLevel(const decltype(SSensorData::idSensor) idSensor, const SData& value) = 0;
    virtual bool setAlarmLevel(const decltype(SSensorData::idSensor) idSensor, const SData& value) = 0;
    virtual bool setTurnOn(const decltype(SSensorData::idSensor) idSensor, const bool turnOn) = 0;
    virtual bool setSaveSDCardIfOnlineEnable(bool enable) = 0;

    virtual ~IConfiguration() = default;
  };

}
#endif // ICONFIGURATION_H
