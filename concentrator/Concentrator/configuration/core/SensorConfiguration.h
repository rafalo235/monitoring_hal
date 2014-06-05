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

    uint8_t idSensor;
    bool turnOn;
    CData warningLvl;
    CData alarmLvl;


    CSensorConfiguration() = default;
  protected:

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

    //! \return true jesli wlaczony, false jesli nie
    virtual bool isTurnOn() const
    {
      return turnOn;
    }

    //! \return poziom ostrzegawczy
    virtual const CData getWarnigLvl() const
    {
      return warningLvl;
    }

    //! \return poziom alarmowy
    virtual const CData getAlarmLvl() const
    {
      return alarmLvl;
    }

    virtual uint8_t getSensorId()
    {
      return idSensor;
    }

    void setTurnOn(bool turnOn1)
    {
      turnOn = turnOn1;
    }

    void setWarningLvl(const CData& data)
    {
      warningLvl = data;
    }

    void setAlarmLvl(const CData& data)
    {
      alarmLvl = data;
    }

  };

}
#endif // SENSORCONFIGURATION_H
