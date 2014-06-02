#ifndef ISENSORSMANAGER_H
#define ISENSORSMANAGER_H

#include "communication/interfaces/protocol.h"

namespace NSensors {

  using namespace NProtocol;

  class ISensorsManager
  {
  public:


    virtual SData getSensorData(const decltype(SSensorData::idSensor) idSensor) const = 0 ;

    virtual ~ISensorsManager() = default;
  };

}

#endif // ISENSORSMANAGER_H
