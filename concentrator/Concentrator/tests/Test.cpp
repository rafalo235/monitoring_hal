#include "Test.h"


namespace NTest{
  /*
  using namespace NProtocol;
  using namespace NEngine;
*/
  CTest::CTest()
  {
   // CConfigurationManager::getInstance()->readConfiguration();
  }
  void CTest::createProtocol()
  {
/*
    SMonitorData monitor;
    monitor.sendTime = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsAmount = 3;
    monitor.sensorsDataSize = 1;
    monitor.sensorsData = new SSensorData[monitor.sensorsDataSize];

    int i = 0;

    monitor.sensorsData[i].idData = i;
    monitor.sensorsData[i].idSensor = 1;
    monitor.sensorsData[i].timeStamp = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsData[i].sensorState = ESensorState::OK;
    monitor.sensorsData[i].dangerLevel = EDangerLevel::NONE;
    monitor.sensorsData[i].data.type = EValueType::INT_8;
    monitor.sensorsData[i].data.value.vInt8 = static_cast<int8_t>(-10);

    ++i;
    monitor.sensorsData[i].idData = i;
    monitor.sensorsData[i].idSensor = 1;
    monitor.sensorsData[i].timeStamp = QDateTime ::currentDateTime().toTime_t();
    monitor.sensorsData[i].sensorState = ESensorState::OK;
    monitor.sensorsData[i].dangerLevel = EDangerLevel::NONE;
    monitor.sensorsData[i].data.type = EValueType::INT_8;
    monitor.sensorsData[i].data.value.vInt8 = static_cast<int8_t>(-20);

    ++i;
    monitor.sensorsData[i].idData = i;
    monitor.sensorsData[i].idSensor = 2;
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

    DCommunication con = CCommunication::getInstance();
    decltype(SProtocol::idPackage) id;
    id = con->sendMonitorData(monitor);

    SServerRequest request;
    request.requestsSize = 2;
    request.requests = new SRequest[request.requestsSize];

    i = 0;
    SRequest r1;
    r1.idSensor = 0;
    r1.configurationType = EConfigurationType::ALARM_LEVEL;
    request.requests[i] = r1;

    SRequest r2;
    r2.idSensor = 0;
    r2.configurationType = EConfigurationType::ALARM_LEVEL;
    request.requests[++i] = r2;

    id = con->sendServerRequest(request);


    i = 0;
    SConfigurationResponse confRes;
    confRes.status = EReceiveStatus::OK;
    confRes.idRequestPackage = 10u;
    SConfiguration conf;
    conf.configurationSize = 2;
    conf.configurations = new SConfigurationValue[conf.configurationSize];

    SConfigurationValue v1;
    v1.idSensor = 0;
    v1.configurationType = EConfigurationType::ALARM_LEVEL;
    SData s1;
    s1.type = EValueType::INT_8;
    s1.value.vInt8 = 1;
    v1.data = s1;
    conf.configurations[i] = v1;

    SConfigurationValue v2;
    v2.idSensor = 1;
    v2.configurationType = EConfigurationType::DANGER_LEVEL;
    SData s2;
    s2.type = EValueType::INT_32;
    s2.value.vInt32 = 1234;
    v2.data = s2;
    conf.configurations[++i] = v2;
    confRes.currentConfiguration = conf;

    id = con->sendConfigurationResponse(confRes);
    */
  }
  void CTest::saveConfigurationFile(){
    /*
    decltype(SProtocol::idConcentrator) idConcentrator = 1;
    uint16_t sendingPeriod = 5 * 60;
    uint16_t checkingSensorPeriod = 1 * 60;
    std::vector<CSensorConfiguration> sensors;
    for(int i = 0; i < 3; ++i){
      bool turnOn = true;
      SData warning;
      warning.type = EValueType::INT_16;
      warning.value.vInt16 = 100;
      SData alarm;
      alarm.type = EValueType::INT_16;
      alarm.value.vInt16 = 200;
      sensors.emplace_back(turnOn, warning, alarm);
    }
    CConfiguration conf(idConcentrator, sendingPeriod, sensors, checkingSensorPeriod);
    DConfigurationManager man = CConfigurationManager::getInstance();
    bool res = man->changeConfiguration(conf);

    LOG_ERROR(res);*/
  }
}
