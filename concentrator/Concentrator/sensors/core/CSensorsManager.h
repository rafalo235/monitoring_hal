#ifndef CSENSORSMANAGER_H
#define CSENSORSMANAGER_H

#include "sensors/interfaces/ISensorsManager.h"
#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/IConfiguration.h"

namespace NSensors{
  class CSensorsManager : public ISensorsManager
  {
  public:
    CSensorsManager() = default;

    virtual bool getSensorData(const NEngine::DSensorConfiguration& conf, CData& value) const;
  };
}
#endif // CSENSORSMANAGER_H
