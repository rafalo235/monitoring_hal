#ifndef CSENSORSMANAGERMOCK_H
#define CSENSORSMANAGERMOCK_H
#define SENSOR_MOCK
#include "sensors/core/CSensorsManager.h"
#include "communication/interfaces/protocol.h"
#include "util/Logger.h"

namespace NSensors{
  class CSensorsManagerMock : public CSensorsManager
  {
  public:
    CSensorsManagerMock()
    {
      LOG_DEBUG("CSensorsManagerMock - constructor.");
    }

    virtual bool getSensorData(const int8_t idSensor, CData& value)
    {
      int buf = 4;
      value.setValue(EValueType::INT_32, &buf);
      return true;
    }
  };
}
#endif // CSENSORSMANAGERMOCK_H
