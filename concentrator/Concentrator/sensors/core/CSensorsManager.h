#ifndef CSENSORSMANAGER_H
#define CSENSORSMANAGER_H

#include "sensors/interfaces/ISensorsManager.h"
#include "communication/interfaces/protocol.h"

namespace NSensors{
  class CSensorsManager : public ISensorsManager
  {
  public:
    CSensorsManager() = default;

    virtual bool getSensorData(const int8_t idSensor, CData& value)
    {
      return true;
    }
  };
}
#endif // CSENSORSMANAGER_H
