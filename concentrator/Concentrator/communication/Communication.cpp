#include "Communication.h"

namespace NProtocol{
  std::shared_ptr<CCommunication> CCommunication::obj;
  decltype(SProtocol::idPackage) CCommunication::idPackageBase = 0;

  CCommunication::CCommunication() {
    qRegisterMetaType<SProtocol>("SProtocol");
    qRegisterMetaType<EConnectionStatus>("EConnectionStatus");
    qRegisterMetaType<EMessageType>("EMessageType");
    qRegisterMetaType<UMessage>("UMessage");
    qRegisterMetaType<uint32_t>("uint32_t");

    CHttpThread *worker = new CHttpThread;
    worker->moveToThread(&workerThread);

    connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(sendProtocol(uint32_t, EMessageType, UMessage)),
            worker, SLOT(sendData(uint32_t, EMessageType, UMessage)));

    connect(worker, SIGNAL(resultReady(EConnectionStatus, SProtocol)), this, SLOT(handleResults(EConnectionStatus, SProtocol)));

    workerThread.start();

  }

  CCommunication::~CCommunication() {
    workerThread.quit();
    workerThread.wait();
  }
  void CCommunication::handleResults(const EConnectionStatus error, const SProtocol& protocol)
  {
    LOG_DEBUG("Sending result: ", static_cast<int>(error));
    LOG_PROTOCOL(protocol);
  }

  decltype(SProtocol::idPackage) CCommunication::sendProtocol(const EMessageType type, const UMessage& message)
  {
    ++idPackageBase;
    sendProtocol(idPackageBase, type, message);
    return idPackageBase;
  }

  decltype(SProtocol::idPackage) CCommunication::sendMonitorData(const SMonitorData& data)
  {
    UMessage message;
    message.monitorData = data;
    return sendProtocol(EMessageType::MONITOR_DATA, message);
  }

  decltype(SProtocol::idPackage) CCommunication::sendConfigurationResponse(const SConfigurationResponse& data)
  {
    UMessage message;
    message.configurationResponse = data;
    return sendProtocol(EMessageType::CONFIGURATION_RESPONSE, message);
  }

  decltype(SProtocol::idPackage) CCommunication::sendServerRequest(const SServerRequest& data)
  {
    UMessage message;
    message.serverRequest = data;
    return sendProtocol(EMessageType::SERVER_REQUEST, message);
  }

}
