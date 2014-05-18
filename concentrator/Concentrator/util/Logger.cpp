#include "Logger.h"

namespace NUtil{
  std::shared_ptr<CLogger> CLogger::logger;
  std::set<std::string> CUnlogFile::turnOffFiles;
#ifdef LOGGER_ENABLE

  std::map<NProtocol::ESensorState, std::string> CLogger::sensorStateEnums;
  std::map<NProtocol::EDangerLevel, std::string> CLogger::dangerLevelEnums;
  std::map<NProtocol::EValueType, std::string> CLogger::valueTypeEnums;
  std::map<NProtocol::EConfigurationType, std::string> CLogger::configurationTypeEnums;
  std::map<NProtocol::EMessageType, std::string> CLogger::messageTypeEnums;
  std::map<NProtocol::EReceiveStatus, std::string> CLogger::receiveStatusEnums;
#endif
}
