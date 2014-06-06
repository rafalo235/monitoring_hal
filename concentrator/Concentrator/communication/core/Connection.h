#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QDebug>
#include <memory>

#include "communication/interfaces/protocol.h"
#include "communication/interfaces/IConnection.h"
#include "communication/core/HttpThread.h"


namespace NProtocol {

  class CConnectionFactory;

  //!
  //! \brief The CConnection class Klasa komunikacji implementujaca NProtocol::IConnection.
  class CConnection : public IConnection
  {
    friend class CConnectionFactory;

    //! \brief Klasa watku
    CHttpThread thread;

    //! \brief licznik pakietow
    static uint32_t idPackageBase;

    //! \brief Domyslny konstruktor
    CConnection() = default;

    //! \deprecated
    //! \brief Bez konstruktora kopiujacego
    CConnection(const CConnection&) = delete;

    //! \deprecated
    //! \brief Bez operatora przypisania
    CConnection& operator=(const CConnection&) = delete;

    //! \brief Dodaje protokol do kolejki wysylania
    //! \param type typ protokolu
    //! \param message typ wiadomosci
    //! \return id pakietu
    uint32_t sendProtocol(
        const EMessageType type, const std::shared_ptr<IMessage>& message);

  public:

    ~CConnection() = default;

    //! IConnection::sendMonitorData(const std::shared_ptr<CMonitorData>&)
    virtual uint32_t sendMonitorData(
        const std::shared_ptr<CMonitorData>& data);

    //! IConnection::sendConfigurationResponse(const std::shared_ptr<CConfigurationResponse>&)
    virtual uint32_t sendConfigurationResponse(
        const std::shared_ptr<CConfigurationResponse>& data);

    //! IConnection::sendServerRequest(const std::shared_ptr<CServerRequest>&)
    virtual uint32_t sendServerRequest(
        const std::shared_ptr<CServerRequest>& data);

    //! IConnection::isAnyResult()
    virtual bool isAnyResult() const{
      return thread.isAnyResult();
    }

    //! IConnection::getResult()
    virtual const DConnectionResult getResult(){
      return  thread.getResult();
    }

    //! IConnection::exit()
    virtual void exit(){
      thread.exit();
    }
  };
}
#endif // COMMUNICATION_H
