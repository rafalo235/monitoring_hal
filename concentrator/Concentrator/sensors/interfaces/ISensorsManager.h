#ifndef ISENSORSMANAGER_H
#define ISENSORSMANAGER_H

#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/IConfiguration.h"

namespace NSensors {

  using namespace NProtocol;

  //!
  //! \brief The ISensorsManager class klasa abstrakcyjna manadzera czujnikow
  class ISensorsManager
  {
  public:

    //!
    //! \brief getSensorData odczytuje dane z czujnikow
    //! \param idSensor id czujnika
    //! \param value odczytana wartosc
    //! \return true jesli odczyt sie udal, false jesli czujnik jest rozwalony
    //!
    virtual bool getSensorData(const NEngine::DSensorConfiguration& conf, CData& value) const = 0 ;

    virtual ~ISensorsManager() = default;
  };

}

#endif // ISENSORSMANAGER_H
