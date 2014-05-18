#include "Test.h"
#include "communication/Communication.h"
#include "communication/protocol.h"


namespace NTest{
  using namespace NProtocol;

  CTest::CTest()
  {
  }
  void CTest::createProtocol()
  {
    /*
    protocol.version = VERSION;
    protocol.crc = 0;
    protocol.size = 0;
    protocol.idConcentrator = 1;
    protocol.idPackage = 100;

    protocol.type = EMessageType::MONITOR_DATA;
*/
    SMonitorData monitor;
    monitor.sendTime = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsAmount = 3;
    monitor.sensorsDataSize = 1;
    monitor.sensorsData = new SSensorData[monitor.sensorsDataSize];

    int i = 0;
    monitor.sensorsData[i].idData = i;
    monitor.sensorsData[i].idSensor = 0;
    monitor.sensorsData[i].timeStamp = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsData[i].sensorState = ESensorState::OK;
    monitor.sensorsData[i].dangerLevel = EDangerLevel::NONE;
    monitor.sensorsData[i].data.type = EValueType::INT_8;
    monitor.sensorsData[i].data.value.vInt8 = static_cast<int8_t>(-10);
  /*
    ++i;
    monitor.sensorsData[i].idData = i;
    monitor.sensorsData[i].idSensor = 0;
    monitor.sensorsData[i].timeStamp = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsData[i].sensorState = ESensorState::OK;
    monitor.sensorsData[i].dangerLevel = EDangerLevel::NONE;
    monitor.sensorsData[i].data.type = EValueType::INT_8;
    monitor.sensorsData[i].data.value.vInt8 = static_cast<int8_t>(-20);

    ++i;
    monitor.sensorsData[i].idData = i;
    monitor.sensorsData[i].idSensor = 1;
    monitor.sensorsData[i].timeStamp = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsData[i].sensorState = ESensorState::OK;
    monitor.sensorsData[i].dangerLevel = EDangerLevel::NONE;
    monitor.sensorsData[i].data.type = EValueType::UINT_8;
    monitor.sensorsData[i].data.value.vUInt8 = static_cast<uint8_t>(50);

    ++i;
    monitor.sensorsData[i].idData = i;
    monitor.sensorsData[i].idSensor = 1;
    monitor.sensorsData[i].timeStamp = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsData[i].sensorState = ESensorState::OK;
    monitor.sensorsData[i].dangerLevel = EDangerLevel::NONE;
    monitor.sensorsData[i].data.type = EValueType::UINT_8;
    monitor.sensorsData[i].data.value.vUInt8 = static_cast<uint8_t>(50);

    ++i;
    monitor.sensorsData[i].idData = i;
    monitor.sensorsData[i].idSensor = 2;
    monitor.sensorsData[i].timeStamp = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsData[i].sensorState = ESensorState::OK;
    monitor.sensorsData[i].dangerLevel = EDangerLevel::NONE;
    monitor.sensorsData[i].data.type = EValueType::INT_32;
    monitor.sensorsData[i].data.value.vInt32 = static_cast<int32_t>(32000);
  */
    DCommunication conf = CCommunication::getInstance();
    UMessage message;
    message.monitorData = monitor;
    decltype(SProtocol::idPackage) id;
    id = conf->sendProtocol(EMessageType::MONITOR_DATA, message);


  }
}
