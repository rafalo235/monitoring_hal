#include "HttpThread.h"
#include <QDataStream>
#include <QByteArray>
#include "util/Logger.h"

namespace NProtocol {

  void CHttpThread::sendData(const SProtocol& protocol)
  {
    QByteArray postData;
    // konwertuj protokol do postaci binarnej tablicy QByteArray
    if (!convertToBinary(postData, protocol)){
      // nieprawidlowy format protokolu
      LOG_ERROR("Sending protocol error. idPackage:", protocol.idPackage);
      emit resultReady(EConnectionStatus::OUTPUT_PROTOCOL_FORMAT_ERROR, protocol);
    }
    else
    {
      // tworzy tymczasowa petle komunikatow
      QEventLoop eventLoop;

      // dla sygnalu QNetworkAccessManager::finished wywolaj QEventLoop::quit
      QNetworkAccessManager mgr;
      QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

      // HTTP
      QNetworkRequest req(QUrl("http://localhost:8080/HallMonitorServer/rest/concentrator/post"));
      // typ MIME
      req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
      // wyslij post'a
      QNetworkReply *reply = mgr.post(req, postData);
      eventLoop.exec(); // czekaj QEventLoop::quit (czyli QNetworkAccessManager::finished)

      if (reply->error() == QNetworkReply::NoError) {
        //success
        LOG_DEBUG("Protocol has been sent successfully. idPackage:", protocol.idPackage);
        SProtocol responseProtocol;
        EConnectionStatus status = EConnectionStatus::NONE;
        // przekonwertuj do struktury
        if (!convertToProtocol(responseProtocol, reply->readAll())){
          // blad struktury protokolu
          status = EConnectionStatus::INPUT_PROTOCOL_FORMAT_ERROR;
          LOG_ERROR("Received protocol error. idPackage:", protocol.idPackage);
        }
        else{
          LOG_DEBUG("Protocol has been received successfully. idPackage:", responseProtocol.idPackage);
        }
        delete reply;
        emit resultReady(status, responseProtocol);
      }
      else {
        //failure
        LOG_ERROR("Protocol sending error. idPackage:", protocol.idPackage, ". Error: ", reply->errorString().toStdString());
        delete reply;
        emit resultReady(EConnectionStatus::CONNECTION_ERROR, protocol);
      }


    }

  }



  bool CHttpThread::convertToBinary(QByteArray& array, const SProtocol& protocol)
  {

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
      convertToBinary(array, protocol.message.configurationResponse);
      break;

    case SERVER_REQUEST:
      break;
    case SERVER_MONITOR_RESPONSE:
      // typ protokolu server -> concentrator
      return false;
      break;
    }
    convertToBinary(array, protocol.crc);


    return true;
  }

  void CHttpThread::convertToBinary(QByteArray& array, const SServerRequest& reqs)
  {
    convertToBinary(array, reqs.requestsSize);
    for(decltype(SServerRequest::requestsSize) i = 0; i < reqs.requestsSize; ++i){
      convertToBinary(array, reqs.requests[i]);
    }
  }

  void CHttpThread::convertToBinary(QByteArray& array, const SRequest& req)
  {
    convertToBinary(array, req.idSensor);
    convertToBinary(array, req.configurationType);

  }

  void CHttpThread::convertToBinary(QByteArray& array, const SConfigurationResponse& confResp)
  {
    convertToBinary(array, confResp.status);
    convertToBinary(array, confResp.idRequestPackage);
    convertToBinary(array, confResp.currentConfiguration);
  }

  void CHttpThread::convertToBinary(QByteArray& array, const SConfiguration& conf)
  {
    convertToBinary(array, conf.configurationSize);
    for(decltype(SConfiguration::configurationSize) i = 0; i < conf.configurationSize; ++i){
      convertToBinary(array, conf.configurations[i]);
    }

  }

  void CHttpThread::convertToBinary(QByteArray& array, const SConfigurationValue& confValue)
  {
    convertToBinary(array, confValue.idSensor);
    convertToBinary(array, confValue.configurationType);
    convertToBinary(array, confValue.data);

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

    for(decltype(SMonitorData::sensorsDataSize) i = 0; i < monitorData.sensorsDataSize; ++i){
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
      break;
    case EValueType::UINT_8:
      convertToBinary(array, data.value.vUInt8);
      break;
    case EValueType::INT_16:
      convertToBinary(array, data.value.vInt16);
      break;
    case EValueType::UINT_16:
      convertToBinary(array, data.value.vUInt16);
      break;
    case EValueType::INT_32:
      convertToBinary(array, data.value.vInt32);
      break;
    case EValueType::UINT_32:
      convertToBinary(array, data.value.vUInt32);
      break;
    case EValueType::INT_64:
      convertToBinary(array, data.value.vInt64);
      break;
    case EValueType::UINT_64:
      convertToBinary(array, data.value.vUInt64);
      break;
    case EValueType::FLOAT_32:
      convertToBinary(array, data.value.vFloat32);
      break;
    case EValueType::DOUBLE_64:
      convertToBinary(array, data.value.vDouble64);
      break;
    case EValueType::VOID:
      convertToBinary(array, data.value.vVoid8);
      break;
    }

  }

  bool CHttpThread::convertToProtocol(SProtocol& protocol, const QByteArray& array)
  {
    CByteWrapper wrapper(array);
    READ_WRAPPER(protocol.version, wrapper);
    READ_WRAPPER(protocol.size, wrapper);
    READ_WRAPPER(protocol.idConcentrator, wrapper);
    READ_WRAPPER(protocol.idPackage, wrapper);
    READ_WRAPPER(protocol.type, wrapper);

    switch(protocol.type){
    case EMessageType::SERVER_MONITOR_RESPONSE:
      if(!convertToProtocol(protocol.message.serverMonitorResponse, wrapper)){
        return false;
      }
      break;
    case EMessageType::MONITOR_DATA:
    case EMessageType::CONFIGURATION_RESPONSE:
    case EMessageType::SERVER_REQUEST:
      return false;
    }

    return true;
  }

  bool CHttpThread::convertToProtocol(SServerResponse& response, CByteWrapper& wrapper)
  {
    READ_WRAPPER(response.status, wrapper);
    READ_WRAPPER(response.idRequestPackage, wrapper);
    return convertToProtocol(response.configuration, wrapper);
  }

  bool CHttpThread::convertToProtocol(SConfiguration& configuration, CByteWrapper& wrapper)
  {
    READ_WRAPPER(configuration.configurationSize, wrapper);
    configuration.configurations = new SConfigurationValue[configuration.configurationSize];

    for(decltype(configuration.configurationSize) i = 0; i < configuration.configurationSize; ++i){
      if (!convertToProtocol(configuration.configurations[i], wrapper))
      {
        return false;
      }
    }
    return true;
  }

  bool CHttpThread::convertToProtocol(SConfigurationValue& confValue, CByteWrapper& wrapper)
  {
    READ_WRAPPER(confValue.idSensor, wrapper);
    READ_WRAPPER(confValue.configurationType, wrapper);
    return convertToProtocol(confValue.data, wrapper);
  }

  bool CHttpThread::convertToProtocol(SData& sdata, CByteWrapper& wrapper)
  {
    READ_WRAPPER(sdata.type, wrapper);
    switch(sdata.type){
    case EValueType::INT_8:
      READ_WRAPPER(sdata.value.vInt8, wrapper);
      break;
    case EValueType::UINT_8:
      READ_WRAPPER(sdata.value.vUInt8, wrapper);
      break;
    case EValueType::INT_16:
      READ_WRAPPER(sdata.value.vInt16, wrapper);
      break;
    case EValueType::UINT_16:
      READ_WRAPPER(sdata.value.vUInt16, wrapper);
      break;
    case EValueType::INT_32:
      READ_WRAPPER(sdata.value.vInt32, wrapper);
      break;
    case EValueType::UINT_32:
      READ_WRAPPER(sdata.value.vUInt32, wrapper);
      break;
    case EValueType::INT_64:
      READ_WRAPPER(sdata.value.vInt64, wrapper);
      break;
    case EValueType::UINT_64:
      READ_WRAPPER(sdata.value.vUInt64, wrapper);
      break;
    case EValueType::FLOAT_32:
      READ_WRAPPER(sdata.value.vFloat32, wrapper);
      break;
    case EValueType::DOUBLE_64:
      READ_WRAPPER(sdata.value.vDouble64, wrapper);
      break;
    case EValueType::VOID:
      READ_WRAPPER(sdata.value.vVoid8, wrapper);
      break;
    default:
      return false;
    }
    return true;
  }
}
