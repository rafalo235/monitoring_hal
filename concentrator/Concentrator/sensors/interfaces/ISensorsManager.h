#ifndef ISENSORSMANAGER_H
#define ISENSORSMANAGER_H

#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/IConfiguration.h"

namespace NSensors {

  using namespace NProtocol;

  class ISensorsManager
  {
  public:


    virtual bool getSensorData(const NEngine::DSensorConfiguration& conf, CData& value) const = 0 ;

    virtual ~ISensorsManager() = default;
  };

}

#endif // ISENSORSMANAGER_H
