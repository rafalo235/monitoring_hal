#ifndef CONNECTIONFACTORY_H
#define CONNECTIONFACTORY_H

#include <memory>
#include "communication/interfaces/IConnection.h"
#include "communication/core/Connection.h"

namespace NProtocol{

  typedef std::shared_ptr<IConnection> DConnection;

  class CConnectionFactory
  {

  public:
    CConnectionFactory() = delete;

    CConnectionFactory(const CConnectionFactory&) = delete;
    CConnectionFactory& operator=(const CConnectionFactory&) = delete;

    //!
    //! \brief getConnection funkcja zwraca smart pointer obiektu komunikacji
    //! \return Obiekt komunikacji
    //!
    static DConnection getInstance(){
      static DConnection connection(new CConnection());
      return connection;
    }
  };
}

#endif // CONNECTIONFACTORY_H
