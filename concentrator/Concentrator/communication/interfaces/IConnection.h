#ifndef ICONNECTION_H
#define ICONNECTION_H

#include <memory>

#include "communication/interfaces/protocol.h"
#include "communication/interfaces/IConnectionResult.h"

namespace NProtocol{

  //! \brief Smart pointer do obiektu, ktory zawiera dane zwrocone przez serwer oraz
  //! ewentualnych bledow komunikacji.
  typedef std::shared_ptr<IConnectionResult> DConnectionResult;

  //!
  //! \brief The IConnection class jest interfejsem komunikacji z serwerem.
  //! \author Marcin Serwach
  class IConnection
  {
  public:

    //! \brief Wysyla dane z czujnikow do serwera
    //! \param[in] data Dane czujnikow
    //! \return Id pakietu
    virtual uint32_t sendMonitorData(
        const std::shared_ptr<CMonitorData>& data) = 0;

    //! \brief Wysyla potwierdzenie zmiany konfiguracji do serwera
    //! \param[in] data Aktualna konfiguracja
    //! \return Id pakietu
    virtual uint32_t sendConfigurationResponse(
        const std::shared_ptr<CConfigurationResponse>& data) = 0;

    //! \brief Wysyla prosbe o wyslanie konfigruacji do serwera
    //! \param[in] data Typy konfiguracji
    //! \return Id pakietu
    virtual uint32_t sendServerRequest(
        const std::shared_ptr<CServerRequest>& data) = 0;

    //! \brief Sprawdza czy sa jakies dane z serwera lub blad polaczenie
    //! \return true, jesli sa dane; false jesli nie
    virtual bool isAnyResult() const = 0;

    //! \brief Pobiera dane otrzymane z serwera
    //! \return Wskaznik do danych
    virtual const DConnectionResult getResult() = 0;

    //! \brief Wylacza watek komunikacji z serwerem
    virtual void exit() = 0;

    //! \brief Domyslny wirtualny destruktor.
    virtual ~IConnection() = default;
  };
}
#endif // ICONNECTION_H
