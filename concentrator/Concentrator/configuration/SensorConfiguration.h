#ifndef SENSORCONFIGURATION_H
#define SENSORCONFIGURATION_H

#include "communication/protocol.h"
#include <fstream>

namespace NEngine {
  using namespace NProtocol;

  class CConfiguration;

  //! \brief Konfiguracja pojedynczego czujnika
  class CSensorConfiguration{

    bool turnOn;
    SData warningLvl;
    SData dangerLvl;

  public:
    CSensorConfiguration(bool turnOn1, SData& warningLvl1, SData& dangerLvl1)
      : turnOn(turnOn1), warningLvl(warningLvl1), dangerLvl(dangerLvl1)
    {
    }

    CSensorConfiguration() : turnOn(false)
    {
      warningLvl.type = EValueType::VOID;
      warningLvl.value.vVoid8 = cVoidValue;
      dangerLvl.type = EValueType::VOID;
      dangerLvl.value.vVoid8 = cVoidValue;

    }

    //! \return true jesli wlaczony, false jesli nie
    bool isTurnOn() const{
      return turnOn;
    }

    //! \return poziom ostrzegawczy
    const SData* getWarnigLvl() const{
      return &warningLvl;
    }

    //! \return poziom alarmowy
    const SData* getDangerLvl() const{
      return &dangerLvl;
    }

    void setTurnOff(bool turnOn1){
      turnOn = turnOn1;
    }

    void setWarningLvl(const SData& data){
      warningLvl = data;
    }

    void setDangerLvl(const SData& data){
      dangerLvl = data;
    }

  };

}
#endif // SENSORCONFIGURATION_H
