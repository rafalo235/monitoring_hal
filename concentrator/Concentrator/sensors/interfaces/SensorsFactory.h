#ifndef CSENSORSFACTORY_H
#define CSENSORSFACTORY_H

#include <memory>

#include "sensors/interfaces/ISensorsManager.h"
#include "sensors/core/CSensorsManager.h"

#ifdef SENSOR_MOCK
#include "sensors/tests/CSensorsManagerMock.h"
#endif
namespace NSensors{

  typedef std::shared_ptr<ISensorsManager> DSensorManager;

  class CSensorFactory
  {

  public:
    CSensorFactory() = delete;
    CSensorFactory& operator=(const CSensorFactory&) = delete;
    CSensorFactory(const CSensorFactory&) = delete;

    static DSensorManager getInstance(){
#ifdef SENSOR_MOCK
       static DSensorManager manager(new CSensorsManagerMock());
#else
       static DSensorManager manager(new CSensorsManager());
#endif


      return manager;
    }
  };
}
#endif // CSENSORSFACTORY_H
