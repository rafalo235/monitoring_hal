#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QDebug>
#include <memory>

#include "communication/interfaces/protocol.h"
#include "communication/interfaces/IConnection.h"
#include "communication/core/HttpThread.h"


namespace NProtocol {

  class CConnectionFactory;

  class CConnection : public IConnection
  {
    friend class CConnectionFactory;

    CHttpThread thread;
    static decltype(SProtocol::idPackage) idPackageBase;

    CConnection();
    CConnection(const CConnection&) = delete;
    CConnection& operator=(const CConnection&) = delete;

    //! \brief Dodaje protokol do kolejki wysylania
    //! \param type typ protokolu
    //! \param message typ wiadomosci
    //! \return id pakietu
    decltype(SProtocol::idPackage) sendProtocol(const EMessageType type, const UMessage& message);

  public:

    ~CConnection();

    //! IConnection::sendMonitorData(SMonitorData)
    virtual decltype(SProtocol::idPackage) sendMonitorData(const SMonitorData& data);

    //! IConnection::sendConfigurationResponse(SConfigurationResponse)
    virtual decltype(SProtocol::idPackage) sendConfigurationResponse(const SConfigurationResponse& data);

    //! IConnection::sendServerRequest(SServerRequest)
    virtual decltype(SProtocol::idPackage) sendServerRequest(const SServerRequest& data);

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
