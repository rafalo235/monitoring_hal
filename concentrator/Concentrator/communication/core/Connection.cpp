#include "communication/core/Connection.h"
#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/ConfigurationFactory.h"
#include "util/Cryptography.h"

namespace NProtocol{

  decltype(SProtocol::idPackage) CConnection::idPackageBase = 0;

  CConnection::CConnection(){

  }

  CConnection::~CConnection() {

  }

  decltype(SProtocol::idPackage) CConnection::sendProtocol(
      const EMessageType type, const UMessage& message)
  {

    SProtocol protocol;
    protocol.version = VERSION;
    protocol.idConcentrator = NEngine::CConfigurationFactory::getInstance()->getIdConcentrator();

    protocol.idPackage = ++idPackageBase;
    protocol.crc = 0;
    protocol.type = type;
    protocol.message = message;
    protocol.size = protocol.getSize();
    const char* data = reinterpret_cast<const char*>(&protocol);
    int dataSize = protocol.size;
    protocol.crc = NUtil::CCryptography::crc16(data, dataSize);

    thread.addToSendingQueue(protocol);
    return idPackageBase;
  }

  decltype(SProtocol::idPackage) CConnection::sendMonitorData(const SMonitorData& data)
  {
    UMessage message;
    message.monitorData = data;
    return sendProtocol(EMessageType::MONITOR_DATA, message);
  }

  decltype(SProtocol::idPackage) CConnection::sendConfigurationResponse(const SConfigurationResponse& data)
  {
    UMessage message;
    message.configurationResponse = data;
    return sendProtocol(EMessageType::CONFIGURATION_RESPONSE, message);
  }

  decltype(SProtocol::idPackage) CConnection::sendServerRequest(const SServerRequest& data)
  {
    UMessage message;
    message.serverRequest = data;
    return sendProtocol(EMessageType::SERVER_REQUEST, message);
  }

}
