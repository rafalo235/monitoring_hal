#ifndef CONFIGURATIONFACTORY_H
#define CONFIGURATIONFACTORY_H

#include <memory>
#include "configuration/interfaces/IConfiguration.h"
#include "configuration/core/Configuration.h"

//! \brief Makro definiujace czy uzywac mock'a czy nie. Zakomentowanie spowoduje uzycie poprawnej implementacji.
#define CONFIG_MOCK

#ifdef CONFIG_MOCK
#include "configuration/tests/ConfigurationMock.h"
#endif

namespace NEngine{

  //! \brief Smart pointer do obiektu klasy odpowiedzialnej za konfiguracje.
  typedef std::shared_ptr<IConfiguration> DConfiguration;

  //! \brief The CConnectionFactory class. Wzorzec projektowy fabryki, ktory tworzy NEngine::DConfiguration.
  //! Gwarantuje, ze zostanie utworzon tylko wylacznie jeden obiekt NEngine::DConfiguration.
  //! \author Marcin Serwach
  class CConfigurationFactory
  {

  public:
    //! \deprecated
    //! \brief Konstruktor usuniety, poniewaz klasa zawiera jedynie funkcje statyczne.
    CConfigurationFactory() = delete;

    //! \deprecated
    //! \brief Konstruktor usuniety, poniewaz klasa zawiera jedynie funkcje statyczne.
    CConfigurationFactory(const CConfigurationFactory&) = delete;

    //! \deprecated
    //! \brief Konstruktor usuniety, poniewaz klasa zawiera jedynie funkcje statyczne.
    CConfigurationFactory& operator=(const CConfigurationFactory&) = delete;

    //!
    //! \brief getInstance Funkcja zwraca smart pointer obiektu konfiguracji.
    //! \return Obiekt konfiguracji
    static DConfiguration getInstance(){
#ifdef CONFIG_MOCK
      static DConfiguration configuration(new CConfigurationMock());
#else
      static DConfiguration configuration(new CConfiguration());
#endif
      return configuration;
    }
  };
}

#endif // CONFIGURATIONFACTORY_H
