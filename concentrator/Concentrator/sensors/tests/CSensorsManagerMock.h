#ifndef CSENSORSMANAGERMOCK_H
#define CSENSORSMANAGERMOCK_H
#define SENSOR_MOCK
#include "sensors/core/CSensorsManager.h"

namespace NSensors{
  class CSensorsManagerMock : public CSensorsManager
  {
  public:
    CSensorsManagerMock() = default;

    virtual SData getSensorData(const decltype(SSensorData::idSensor) idSensor) const{
      SData s;
      s.type = EValueType::INT_32;
      s.value.vInt32 = idSensor;
      return s;
    }
  };
}
#endif // CSENSORSMANAGERMOCK_H
