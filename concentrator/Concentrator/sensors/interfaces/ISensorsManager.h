#ifndef ISENSORSMANAGER_H
#define ISENSORSMANAGER_H

#include "communication/interfaces/protocol.h"

namespace NSensors {

  using namespace NProtocol;

  class ISensorsManager
  {
  public:

    //!
    //! \brief getSensorData odczytuje dane z czujnikow
    //! \param idSensor id czujnika
    //! \param value odczytana wartosc
    //! \return true jesli odczyt sie udal, false jesli czujnik jest rozwalony
    //!
    virtual bool getSensorData(const int8_t idSensor, CData& value) = 0;

    virtual ~ISensorsManager() = default;
  };

}

#endif // ISENSORSMANAGER_H
