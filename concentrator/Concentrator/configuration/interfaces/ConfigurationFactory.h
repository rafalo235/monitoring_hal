#ifndef CONFIGURATIONFACTORY_H
#define CONFIGURATIONFACTORY_H

#include <memory>
#include "configuration/interfaces/IConfiguration.h"
#include "configuration/core/Configuration.h"

#define CONFIG_MOCK

#ifdef CONFIG_MOCK
#include "configuration/tests/ConfigurationMock.h"
#endif

namespace NEngine{

  typedef std::shared_ptr<IConfiguration> DConfiguration;

  class CConfigurationFactory
  {

  public:
    CConfigurationFactory() = delete;
    CConfigurationFactory(const CConfigurationFactory&) = delete;
    CConfigurationFactory& operator=(const CConfigurationFactory&) = delete;

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
