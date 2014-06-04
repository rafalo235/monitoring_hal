#ifndef CONFIGURATIONMOCK_H
#define CONFIGURATIONMOCK_H
#define CONFIG_MOCK

#include "configuration/core/Configuration.h"

namespace NEngine{

  //!
  //! \brief The CConfigurationMock class tworzy mock
  //!   dla configuracji. Aby użyc klasy należy zdefiniować CONFIG_MOCK.
  //!
  class CConfigurationMock : public CConfiguration
  {
  public:
    CConfigurationMock();

  };
}
#endif // CONFIGURATIONMOCK_H
