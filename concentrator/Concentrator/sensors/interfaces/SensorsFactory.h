#ifndef CSENSORSFACTORY_H
#define CSENSORSFACTORY_H

#include <memory>

#include "sensors/interfaces/ISensorsManager.h"
#include "sensors/core/CSensorsManager.h"

namespace NSensors{

  typedef std::shared_ptr<ISensorsManager> DSensorManager;

  class CSensorFactory
  {

  public:
    CSensorFactory() = delete;
    CSensorFactory& operator=(const CSensorFactory&) = delete;
    CSensorFactory(const CSensorFactory&) = delete;

    static DSensorManager getInstance(){
      static DSensorManager manager(new CSensorsManager());

      return manager;
    }
  };
}
#endif // CSENSORSFACTORY_H
