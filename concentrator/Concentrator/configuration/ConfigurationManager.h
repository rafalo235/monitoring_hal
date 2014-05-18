#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "configuration/Configuration.h"
#include <memory>

namespace NEngine{

  //! \brief Klasa odczytuje z pliku konfiguracje. Konfiguracja musi byc zapisywalna.
  class CConfigurationManager
  {
    static std::shared_ptr<CConfigurationManager> manager;

    CConfiguration conf;
    CConfigurationManager() = default;
    CConfigurationManager(const CConfigurationManager&) = delete;
    CConfigurationManager& operator=(const CConfigurationManager&) = delete;

  public:

    //! \return zwraca smart pointer do instacji manager'a - singleton
    static std::shared_ptr<CConfigurationManager> getInstance(){
      if (manager.use_count() == 0){
        manager.reset(new CConfigurationManager());
      }
      return manager;
    }

    //! \brief Wczytuje konfiguracje z pliku
    //! \return true jesli wszystko ok, false jesli nie
    bool readConfiguration(){
      std::ifstream file("configuration.conf");
      if (!file.is_open()){
        return false;
      }
      file>>conf;
      file.close();
      return true;
    }

    //! \brief Zmienia konfiguracje i zapisuje ja do pliku
    //! \return true jesli wszystko ok, false jesli nie
    bool changeConfiguration(const CConfiguration& configuration){
      conf = configuration;
      std::ofstream file("configuration.conf");
      if (!file.is_open()){
        return false;
      }
      file<<conf;
      file.close();
      return true;
    }

    const CConfiguration* getConfiguration(){
      return &conf;
    }

  };
}
#endif // CONFIGURATIONMANAGER_H
