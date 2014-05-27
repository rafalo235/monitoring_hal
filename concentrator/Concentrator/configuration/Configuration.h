#ifndef CCONFIGURATION_H
#define CCONFIGURATION_H

#include <fstream>
#include <cstdint>
#include <vector>

#include "communication/protocol.h"
#include "configuration/SensorConfiguration.h"

namespace NEngine {

  using namespace NProtocol;
  class CConfiguration
  {

    //! \brief ID koncentratora do wysylania
    decltype(SProtocol::idConcentrator) idConcentrator;

    //! \brief Przerwa pomiedzy wysylanymi danymi
    uint16_t sendingPeriod;

    //! \brief Przerwa pomiedzy sprawdzaniem danych z czujnikow
    uint16_t checkingSensorPeriod;

    //! \brief Konfiguracja dla poszczegolnych sensorow
    std::vector<CSensorConfiguration> sensors;


  public:
    CConfiguration() = default;

    CConfiguration(const decltype(SProtocol::idConcentrator) idConcentrator1,
                   const uint16_t sendingPeriod1,
                   const std::vector<CSensorConfiguration> sensors1,
                   const uint16_t checkingSensorPeriod1):
      idConcentrator(idConcentrator1), sendingPeriod(sendingPeriod1), checkingSensorPeriod(checkingSensorPeriod1), sensors(sensors1)
    {

    }

    //! \return Zwraca przerwe pomiedzy sprawdzaniem danych z czujnikow
    uint16_t getCheckingSensorPeriod() const{
      return checkingSensorPeriod;
    }

    //! \return Zwraca id koncentratora odczytane z pliku konfiguracyjnego
    decltype(SProtocol::idConcentrator) getIdConcentrator() const{
      return idConcentrator;
    }

    //! \return Zwraca przerwe pomiedzy wysylanymi danymi
    uint16_t getSendingPeriod() const{
      return sendingPeriod;
    }

    //! \return Zwraca konfiguracje czujnikow
    const std::vector<CSensorConfiguration>* getSensorConfiguration()const{
      return &sensors;
    }


  };
}
#endif // CCONFIGURATION_H
