#ifndef CENGINEFACTORY_H
#define CENGINEFACTORY_H

#include <memory>

#include "engine/core/Engine.h"
#include "engine/interfaces/IEngine.h"

namespace NEngine{
  typedef std::shared_ptr<IEngine> DEngine;

  class CEngineFactory
  {

  public:
    CEngineFactory() = delete;

    static DEngine getInstance(){
      static DEngine engine(new CEngine());
      return engine;
    }
  };
}
#endif // ENGINEFACTORY_H
