#include "HttpThread.h"
#include <QDataStream>
#include <QByteArray>
namespace NProtocol {

  QByteArray CHttpThread::convertToBinary(const SProtocol& protocol)
  {

    QByteArray array;
    array.reserve(protocol.size);
    convertToBinary(array, protocol.version);
    convertToBinary(array, protocol.size);
    convertToBinary(array, protocol.idConcentrator);
    convertToBinary(array, protocol.idPackage);
    convertToBinary(array, protocol.type);

    switch(protocol.type){
    case MONITOR_DATA:
      convertToBinary(array, protocol.message.monitorData);
      break;
    case CONFIGURATION_RESPONSE:
      break;
    case SERVER_MONITOR_RESPONSE:
      break;
    case SERVER_REQUEST:
      break;
    }
    convertToBinary(array, protocol.crc);
    char* data = array.data();
    for(int i = 0; i < protocol.size; ++i){
      qDebug()<< (int)data[i];
    }
    return array;
  }

  template<typename T>
  void CHttpThread::convertToBinary(QByteArray& array, const T& data){
    array.append((const char*)&data, sizeof(data));
  }

  void CHttpThread::convertToBinary(QByteArray& array, const SMonitorData& monitorData)
  {
    convertToBinary(array, monitorData.sendTime);
    convertToBinary(array, monitorData.sensorsAmount);
    convertToBinary(array, monitorData.sensorsDataSize);

    for(uint32_t i = 0; i < monitorData.sensorsDataSize; ++i){
      convertToBinary(array, monitorData.sensorsData[i]);
    }
  }

  void CHttpThread::convertToBinary(QByteArray& array, const SSensorData& sensorData)
  {

    convertToBinary(array, sensorData.idData);
    convertToBinary(array, sensorData.idSensor);
    convertToBinary(array, sensorData.timeStamp);
    convertToBinary(array, sensorData.sensorState);
    convertToBinary(array, sensorData.dangerLevel);

    convertToBinary(array, sensorData.data);
  }

  void CHttpThread::convertToBinary(QByteArray& array, const SData& data)
  {
    convertToBinary(array, data.type);


    switch(data.type){
    case EValueType::INT_8:
      convertToBinary(array, data.value.vInt8);
    case EValueType::UINT_8:

      convertToBinary(array, data.value.vUInt8);
    case EValueType::INT_16:

      convertToBinary(array, data.value.vInt16);
    case EValueType::UINT_16:

      convertToBinary(array, data.value.vUInt16);
    case EValueType::INT_32:

      convertToBinary(array, data.value.vInt32);
    case EValueType::UINT_32:

      convertToBinary(array, data.value.vUInt32);
    case EValueType::INT_64:

      convertToBinary(array, data.value.vInt64);
    case EValueType::UINT_64:

      convertToBinary(array, data.value.vUInt64);
    case EValueType::FLOAT_32:

      convertToBinary(array, data.value.vFloat32);
    case EValueType::DOUBLE_64:

      convertToBinary(array, data.value.vDouble64);
    case EValueType::VOID:

      convertToBinary(array, data.value.vVoid8);
    }

  }
}
