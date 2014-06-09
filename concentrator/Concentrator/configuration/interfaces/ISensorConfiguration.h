#ifndef ISENSORCONFIGURATION_H
#define ISENSORCONFIGURATION_H

#include <vector>
#include <cstdint>

#include "communication/interfaces/protocol.h"

namespace NEngine{
  using namespace NProtocol;

  //!
  //! \brief The ISensorConfiguration class Klasa abstrakcyjna konfiguracji pojedynczego czujnika
  class ISensorConfiguration
  {
  public:

    //!
    //! \brief getSensorId Zwraca id czujnika
    //! \return id czujnika
    virtual uint8_t getSensorId() = 0;

    //!
    //! \brief isTurnOn Zwraca informacje czy czujnik jest wlaczony
    //! \return true jesli czujnik wlaczony; false jesli nie
    virtual bool isTurnOn() const = 0;

    //!
    //! \brief getWarnigLvl Zwraca minimalna wartosc oszczegawcza
    //! \return minimalna wartosc ostrzegawcza
    virtual const CData getWarnigLvl() const = 0;

    //!
    //! \brief getAlarmLvl Zwraca minimalna wartosc alarmowa
    //! \return  minimalna wartosc alarmaowa
    virtual const CData getAlarmLvl() const = 0;
    virtual std::string getIpv4Address() const = 0;
    virtual int getPort() const = 0;

    //! \brief domyslny dekonstruktor
    virtual ~ISensorConfiguration() = default;
  };
}
#endif // ISENSORCONFIGURATION_H
