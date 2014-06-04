#ifndef CSENSORSMANAGER_H
#define CSENSORSMANAGER_H

#include "sensors/interfaces/ISensorsManager.h"
#include "communication/interfaces/protocol.h"

namespace NSensors{
  class CSensorsManager : public ISensorsManager
  {
  public:
    CSensorsManager() = default;

    virtual SData getSensorData(const decltype(SSensorData::idSensor) idSensor) const{

      // TODO: kto to zrobi?
      SData s;
      return s;
    }
  };
}
#endif // CSENSORSMANAGER_H
