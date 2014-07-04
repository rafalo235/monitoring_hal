#ifndef CENGINEFACTORY_H
#define CENGINEFACTORY_H

#include <memory>

#include "engine/core/Engine.h"
#include "engine/interfaces/IEngine.h"

//! \brief NEngine namespace uzywany do calej logiki i konfiguracji.
namespace NEngine{

  //! \brief Smart pointer do Silnika
  typedef std::shared_ptr<IEngine> DEngine;


  //! \brief The CEngineFactory class wzorzec fabryki tworzacy instacje silnika
  //! \author Marcin Serwach
  class CEngineFactory
  {

  public:
    CEngineFactory() = delete;
    CEngineFactory(const CEngineFactory& obj) = delete;
    CEngineFactory& operator=(const CEngineFactory& obj) = delete;

    //!
    //! \brief getInstance pobiera instancje silnika.
    //! \return Smart pointer do silnika
    static DEngine getInstance(){
      static DEngine engine(new CEngine());
      return engine;
    }
  };
}
#endif // ENGINEFACTORY_H
