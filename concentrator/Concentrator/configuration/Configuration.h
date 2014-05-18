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

    std::vector<CSensorConfiguration> sensors;


  public:
    CConfiguration() = default;

    CConfiguration(const decltype(SProtocol::idConcentrator) idConcentrator1,
                   const uint16_t sendingPeriod1,
                   const std::vector<CSensorConfiguration> sensors1):
      idConcentrator(idConcentrator1), sendingPeriod(sendingPeriod1), sensors(sensors1)
    {

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
    const std::vector<CSensorConfiguration>* getSensorConfiguration(){
      return &sensors;
    }

    friend std::ifstream& operator >>(std::ifstream& stream, CConfiguration& obj);
    friend std::ofstream& operator <<(std::ofstream& stream, const CConfiguration& obj);

  };
}
#endif // CCONFIGURATION_H
