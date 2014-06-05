#ifndef SENSORCONFIGURATION_H
#define SENSORCONFIGURATION_H

#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/ISensorConfiguration.h"


namespace NEngine {
  using namespace NProtocol;
  class CConfiguration;


  //! \brief Konfiguracja pojedynczego czujnika
  class CSensorConfiguration : public ISensorConfiguration
  {
    friend class CConfiguration;

    //! \brief id czujnika
    uint8_t idSensor;

    //! \brief true jesli czujnik wlaczony; false jesli wylaczony
    bool turnOn;

    //! \brief minimalna wartosc wymagajaca ostrzezenia
    CData warningLvl;

    //! \brief minimalna wartosc alarmowa
    CData alarmLvl;

    //! \brief domyslny koncentrator
    CSensorConfiguration() = default;
  protected:

    //!
    //! \brief CSensorConfiguration konstruktor wykorzystywany przez mock; pozniej mozna go usunac
    //! \param idSensor1 id czujnika
    //! \param turnOn1 wlaczony/wylaczony
    //! \param warningLvl1 wartosc ostrzezenia
    //! \param alarmLvl1 wartosci alarmowa
    CSensorConfiguration(uint8_t& idSensor1,
                         bool& turnOn1,
                         CData& warningLvl1,
                         CData& alarmLvl1) :
                            idSensor(idSensor1),
                            turnOn(turnOn1),
                            warningLvl(warningLvl1),
                            alarmLvl(alarmLvl1)
    {

    }

  public:

    CSensorConfiguration& operator=(const CSensorConfiguration&) = delete;
    CSensorConfiguration(const CSensorConfiguration&) = default;

    //! ISensorConfiguration::isTurnOn()
    virtual bool isTurnOn() const
    {
      return turnOn;
    }

    //! ISensorConfiguration::getWarnigLvl()
    virtual const CData getWarnigLvl() const
    {
      return warningLvl;
    }

    //! ISensorConfiguration::getAlarmLvl()
    virtual const CData getAlarmLvl() const
    {
      return alarmLvl;
    }

    //! ISensorConfiguration::getSensorId()
    virtual uint8_t getSensorId()
    {
      return idSensor;
    }

    //! ISensorConfiguration::isTurnOn(bool)
    void setTurnOn(bool turnOn1)
    {
      turnOn = turnOn1;
    }

    //! ISensorConfiguration::setWarningLvl(const CData&)
    void setWarningLvl(const CData& data)
    {
      warningLvl = data;
    }

    //! ISensorConfiguration::setAlarmLvl(const CData& )
    void setAlarmLvl(const CData& data)
    {
      alarmLvl = data;
    }

  };

}
#endif // SENSORCONFIGURATION_H
