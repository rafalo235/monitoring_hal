#ifndef CONFIGURATIONFACTORY_H
#define CONFIGURATIONFACTORY_H

#include <memory>
#include "configuration/interfaces/IConfiguration.h"
#include "configuration/core/Configuration.h"

namespace NEngine{
  typedef std::shared_ptr<IConfiguration> DConfiguration;

  class CConfigurationFactory
  {

  public:
    CConfigurationFactory() = delete;
    CConfigurationFactory(const CConfigurationFactory&) = delete;
    CConfigurationFactory& operator=(const CConfigurationFactory&) = delete;

    static DConfiguration getInstance(){
      static DConfiguration configuration(new CConfiguration());

      return configuration;
    }


  };
}

#endif // CONFIGURATIONFACTORY_H
