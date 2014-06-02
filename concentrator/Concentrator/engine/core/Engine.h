#ifndef ENGINE_H
#define ENGINE_H

#include "engine/interfaces/IEngine.h"

#include "engine/core/CycleMonitor.h"
#include "communication/interfaces/ConnectionFactory.h"
#include "sensors/interfaces/SensorsFactory.h"
#include "configuration/interfaces/ConfigurationFactory.h"
#include "util/Logger.h"

namespace NEngine{

  using namespace NSensors;
  using namespace NProtocol;

  //! \brief Klasa odpowiedzialna za uruchomienie wszystkich uslug
  class CEngine : public IEngine
  {
    friend class EngineFactory;

    DConfiguration configuration;

    CCycleMonitor cycleMonitor;
  public:


    virtual bool init(){
      configuration = CConfigurationFactory::getInstance();
      if (!configuration->readConfiguration()){
        LOG_ERROR("Reading configuration failed");
      }
      cycleMonitor.runThread();

      return true;
    }

    virtual void exit(){
      CConnectionFactory::getInstance()->exit();
      cycleMonitor.exit();
    }
  };
}
#endif // ENGINE_H
