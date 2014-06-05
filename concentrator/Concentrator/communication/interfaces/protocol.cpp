#include "protocol.h"

namespace NProtocol {

  const uint32_t CData::headerSize = sizeof(type);

  const uint32_t CSensorData::headerSize = sizeof(idData) + sizeof(idSensor) +
                                     sizeof(timeStamp) +
                                     sizeof(sensorState) + sizeof(dangerLevel);

  const uint32_t CMonitorData::headerSize = sizeof(sendTime) + sizeof(sensorsAmount) +
                                      sizeof(uint32_t);

  const uint32_t CConfigurationValue::headerSize = sizeof(idSensor) + sizeof(configurationType);

  const uint32_t CConfiguration::headerSize = sizeof(uint8_t);

  const uint32_t CConfigurationResponse::headerSize = sizeof(status) + sizeof(idRequestPackage);

  const uint32_t CRequest::headerSize = sizeof(idSensor) + sizeof(configurationType);
  const uint32_t CServerRequest::headerSize = sizeof(uint8_t);
  const uint32_t CServerResponse::headerSize = sizeof(status) + sizeof(idRequestPackage);
  const uint32_t CProtocol::headerSize = sizeof(version) + sizeof(size) +
                                   sizeof(idConcentrator) + sizeof(idPackage) + sizeof(type) + sizeof(crc);

}
