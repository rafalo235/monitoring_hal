#ifndef CSENSORSMANAGERMOCK_H
#define CSENSORSMANAGERMOCK_H
#define SENSOR_MOCK
#include "sensors/core/CSensorsManager.h"
#include "util/Logger.h"

namespace NSensors{
  class CSensorsManagerMock : public CSensorsManager
  {
  public:
    CSensorsManagerMock()
    {
      LOG_DEBUG("CSensorsManagerMock - constructor.");
    }

    virtual CData getSensorData(const int8_t idSensor) const{
      int buf = 4;
      return CData(EValueType::INT_32, &buf);
    }
  };
}
#endif // CSENSORSMANAGERMOCK_H
