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
    static uint32_t idPackageBase;

    CConnection();
    CConnection(const CConnection&) = delete;
    CConnection& operator=(const CConnection&) = delete;

    //! \brief Dodaje protokol do kolejki wysylania
    //! \param type typ protokolu
    //! \param message typ wiadomosci
    //! \return id pakietu
    uint32_t sendProtocol(
        const EMessageType type, const std::shared_ptr<IMessage>& message);

  public:

    ~CConnection();

    //! IConnection::sendMonitorData(SMonitorData)
    virtual uint32_t sendMonitorData(
        const std::shared_ptr<CMonitorData>& data);

    //! IConnection::sendConfigurationResponse(SConfigurationResponse)
    virtual uint32_t sendConfigurationResponse(
        const std::shared_ptr<CConfigurationResponse>& data);

    //! IConnection::sendServerRequest(SServerRequest)
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
