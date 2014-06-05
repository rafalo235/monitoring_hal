#ifndef CSENSORSMANAGER_H
#define CSENSORSMANAGER_H

#include "sensors/interfaces/ISensorsManager.h"
#include "communication/interfaces/protocol.h"

namespace NSensors{
  class CSensorsManager : public ISensorsManager
  {
  public:
    CSensorsManager() = default;

    virtual CData getSensorData(const int8_t idSensor) const{

      // TODO: kto to zrobi?
      return CData();
    }
  };
}
#endif // CSENSORSMANAGER_H
