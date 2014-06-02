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

    decltype(SSensorData::idSensor) idSensor;
    bool turnOn;
    SData warningLvl;
    SData alarmLvl;

    CSensorConfiguration() = default;
  public:

    CSensorConfiguration& operator=(const CSensorConfiguration&) = delete;
    CSensorConfiguration(const CSensorConfiguration&) = default;

    //! \return true jesli wlaczony, false jesli nie
    virtual bool isTurnOn() const{
      return turnOn;
    }

    //! \return poziom ostrzegawczy
    virtual const SData getWarnigLvl() const{
      return warningLvl;
    }

    //! \return poziom alarmowy
    virtual const SData getAlarmLvl() const{
      return alarmLvl;
    }

    virtual decltype(SSensorData::idSensor) getSensorId()
    {
      return idSensor;
    }

    void setTurnOn(bool turnOn1){
      turnOn = turnOn1;
    }

    void setWarningLvl(const SData& data){
      warningLvl = data;
    }

    void setAlarmLvl(const SData& data){
      alarmLvl = data;
    }

  };

}
#endif // SENSORCONFIGURATION_H
