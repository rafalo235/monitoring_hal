#ifndef ISENSORSMANAGER_H
#define ISENSORSMANAGER_H

#include "communication/interfaces/protocol.h"

namespace NSensors {

  using namespace NProtocol;

  class ISensorsManager
  {
  public:


    virtual CData getSensorData(const int8_t idSensor) const = 0 ;

    virtual ~ISensorsManager() = default;
  };

}

#endif // ISENSORSMANAGER_H
