#ifndef ENGINE_H
#define ENGINE_H

#include "configuration/ConfigurationManager.h"
#include "util/Logger.h"

namespace NEngine{

  //! \brief Klasa odpowiedzialna za uruchomienie wszystkich uslug
  class CEngine
  {
  public:
    CEngine();

    bool init(){
      DConfigurationManager conf = CConfigurationManager::getInstance();
      if (!conf->readConfiguration()){
        LOG_ERROR("Configuration reading error");
        return false;
      }
      return true;
    }
  };
}
#endif // ENGINE_H
