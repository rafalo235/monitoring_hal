#ifndef CONNECTIONFACTORY_H
#define CONNECTIONFACTORY_H

#include <memory>
#include "communication/interfaces/IConnection.h"
#include "communication/core/Connection.h"

//! \brief NProtocol jest przestrzenia nazw klas odpowiedzialnych za wysylanie danych.
namespace NProtocol{

  //! \brief Smart pointer do obiektu klasy odpowiedzialnej za komunikacje.
  typedef std::shared_ptr<IConnection> DConnection;

  //!
  //! \brief The CConnectionFactory class. Wzorzec projektowy fabryki, ktory tworzy NProtocol::DConnection.
  //! Gwarantuje, ze zostanie utworzon tylko wylacznie jeden obiekt NProtocol::DConnection.
  //! \author Marcin Serwach
  class CConnectionFactory
  {

  public:

    //! \deprecated
    //! \brief Konstruktor usuniety, poniewaz klasa zawiera jedynie funkcje statyczne.
    CConnectionFactory() = delete;

    //! \deprecated
    //! \brief Konstruktor usuniety, poniewaz klasa zawiera jedynie funkcje statyczne.
    CConnectionFactory(const CConnectionFactory&) = delete;

    //! \deprecated
    //! \brief Konstruktor usuniety, poniewaz klasa zawiera jedynie funkcje statyczne.
    CConnectionFactory& operator=(const CConnectionFactory&) = delete;

    //!
    //! \brief getInstance Funkcja zwraca smart pointer obiektu komunikacji.
    //! \return Obiekt komunikacji
    static DConnection getInstance(){
      static DConnection connection(new CConnection());
      return connection;
    }
  };
}

#endif // CONNECTIONFACTORY_H
