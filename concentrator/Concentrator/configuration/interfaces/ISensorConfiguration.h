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

    virtual uint8_t getSensorId() = 0;
    virtual bool isTurnOn() const = 0;
    virtual const CData getWarnigLvl() const = 0;
    virtual const CData getAlarmLvl() const = 0;
    virtual std::string getIpv4Address() const = 0;
    virtual int getPort() const = 0;

    virtual ~ISensorConfiguration() = default;
  };
}
#endif // ISENSORCONFIGURATION_H
