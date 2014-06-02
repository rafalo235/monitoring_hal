#ifndef ISENSORCONFIGURATION_H
#define ISENSORCONFIGURATION_H

#include <vector>
#include <cstdint>

#include "communication/interfaces/protocol.h"

namespace NEngine{
  using namespace NProtocol;

  class ISensorConfiguration
  {
  public:

    virtual decltype(SSensorData::idSensor) getSensorId() = 0;
    virtual bool isTurnOn() const = 0;
    virtual const SData getWarnigLvl() const = 0;
    virtual const SData getAlarmLvl() const = 0;

    virtual ~ISensorConfiguration() = default;
  };
}
#endif // ISENSORCONFIGURATION_H
