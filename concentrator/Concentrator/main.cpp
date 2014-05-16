#include <QCoreApplication>

#include "communication/protocol.h"
#include "communication/HttpThread.h"
#include "communication/Communication.h"
#include <iostream>

using namespace NProtocol;
/*
void fun2(const char* data, int size){

  std::cout<<"size: "<<size;
}

template<typename T>
void fun(const T& data){

  fun2((const char*)&data, sizeof(T));
}
*/
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);


  SProtocol protocol;
  protocol.version = VERSION;
  protocol.crc = 0;
  protocol.size = 0;
  protocol.idConcentrator = 1;
  protocol.idPackage = 100;
  protocol.type = EMessageType::MONITOR_DATA;

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
  protocol.message.monitorData = monitor;

  uint32_t size = protocol.getSize();
  protocol.size = size;

  std::cout<<size<<std::endl;


  CCommunication cont;

  cont.sendProtocol(protocol);


  return a.exec();
}
