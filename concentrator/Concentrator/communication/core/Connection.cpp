#include "communication/core/Connection.h"
#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/ConfigurationFactory.h"
#include "util/Cryptography.h"

namespace NProtocol{

  uint32_t CConnection::idPackageBase = 0;

  CConnection::CConnection(){

  }

  CConnection::~CConnection() {

  }

  uint32_t CConnection::sendProtocol(
      const EMessageType type, const std::shared_ptr<IMessage>& message)
  {

    uint16_t idConcentrator = NEngine::CConfigurationFactory::getInstance()->getIdConcentrator();
    CProtocol protocol(VERSION, 0,
                       idConcentrator,
                       ++idPackageBase, type, message);

    const char* data = reinterpret_cast<const char*>(&protocol);
    int dataSize = protocol.getSize();
    protocol.setCRC(NUtil::CCryptography::crc16(data, dataSize));

    thread.addToSendingQueue(protocol);
    return idPackageBase;
  }

  uint32_t CConnection::sendMonitorData(const std::shared_ptr<CMonitorData>& data)
  {
    return sendProtocol(EMessageType::MONITOR_DATA, std::dynamic_pointer_cast<IMessage>(data));
  }

  uint32_t CConnection::sendConfigurationResponse(const std::shared_ptr<CConfigurationResponse>& data)
  {
    return sendProtocol(EMessageType::CONFIGURATION_RESPONSE, std::dynamic_pointer_cast<IMessage>(data));
  }

  uint32_t CConnection::sendServerRequest(const std::shared_ptr<CServerRequest>& data)
  {
    return sendProtocol(EMessageType::SERVER_REQUEST, std::dynamic_pointer_cast<IMessage>(data));
  }

}
