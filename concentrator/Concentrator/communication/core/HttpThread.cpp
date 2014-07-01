#include <memory>
#include <QDataStream>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <algorithm>
#include <string>

#include "HttpThread.h"

#include "util/Logger.h"
#include "util/Cryptography.h"

#include "communication/core/ConnectionTask.h"
#include "communication/core/ConnectionResult.h"
#include "configuration/interfaces/ConfigurationFactory.h"
#include "util/Cryptography.h"

namespace NProtocol {

  CHttpThread::CHttpThread(){

    thread.reset(new std::thread([&](){run();}));
  }

  void CHttpThread::run(){
    bool exit = false;
    do{
      const DConnectionTask task = sendingQueue.pop();
      if (task->getType() == IConnectionTask::EConnectionTaskType::EXIT)
      {
        exit = true;
      }
      else if (task->getType() == IConnectionTask::EConnectionTaskType::SENDING)
      {
        std::shared_ptr<CSendingTask> sendingTask = std::dynamic_pointer_cast<CSendingTask>(task);
        sendHttp(sendingTask->getProtocol());
      }

    }while(!exit);
  }

  void CHttpThread::addToSendingQueue(const CProtocol& prot)
  {
    DConnectionTask task(new CSendingTask(prot));
    sendingQueue.push(task);

  }

  void CHttpThread::exit()
  {
    DConnectionTask task(new CExitTask());
    sendingQueue.push(task);
    thread->join();
  }


  void CHttpThread::sendHttp(const CProtocol& protocol)
  {

    QByteArray postData;
    // konwertuj protokol do postaci binarnej tablicy QByteArray
    if (!convertToBinary(postData, protocol)){
      // nieprawidlowy format protokolu
      LOG_ERROR("Sending protocol error. idPackage:", protocol.getIdPackage());
      DConnectionResult res(new CConnectionResult(protocol, EConnectionStatus::OUTPUT_PROTOCOL_FORMAT_ERROR));
      resultsQueue.push(res);
    }
    else
    {

      uint16_t crc = NUtil::CCryptography::crc16(postData.constData(), postData.size());
      postData.replace(postData.size() - sizeof(crc), sizeof(crc), reinterpret_cast<char*>(&crc), sizeof(crc));

      // tworzy tymczasowa petle komunikatow
      QEventLoop eventLoop;

      // dla sygnalu QNetworkAccessManager::finished wywolaj QEventLoop::quit
      QNetworkAccessManager mgr;
      QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

      // HTTP
      const std::string url = NEngine::CConfigurationFactory::getInstance()->getServerUrl();
      QUrl qUrl(url.c_str());
      QNetworkRequest req(qUrl);
      // typ MIME
      req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
      // wyslij post'a
      std::shared_ptr<QNetworkReply> reply(mgr.post(req, postData));
      eventLoop.exec(); // czekaj QEventLoop::quit (czyli QNetworkAccessManager::finished)

      if (reply->error() == QNetworkReply::NoError) {
        //success
        LOG_DEBUG("Protocol has been sent successfully. idPackage:", protocol.getIdPackage());
        CByteWrapper wrapper(reply->readAll());

        if (!wrapper.isCRCValid())
        {
          LOG_ERROR("Received protocol error - CRC. idPackage:", protocol.getIdPackage());
          DConnectionResult res(new CConnectionResult(protocol, EConnectionStatus::CRC_ERROR));
          resultsQueue.push(res);

        }
        else
        {
          std::shared_ptr<CProtocol> responseProtocol =
              convertToProtocol(wrapper);
          // przekonwertuj do struktury
          if (!responseProtocol){
            // blad struktury protokolu
            LOG_ERROR("Received protocol error. idPackage:", protocol.getIdPackage());
            DConnectionResult res(new CConnectionResult(protocol, EConnectionStatus::INPUT_PROTOCOL_FORMAT_ERROR));
            resultsQueue.push(res);
          }
          else{
            LOG_DEBUG("Protocol has been received successfully. idPackage:", responseProtocol->getIdPackage());
            DConnectionResult res(new CConnectionResult(protocol, responseProtocol, EConnectionStatus::NONE));
            resultsQueue.push(res);
          }
        }

      }
      else {

        LOG_ERROR("Protocol sending error. idPackage:",
                  protocol.getIdPackage(), ". Error: ", reply->errorString().toStdString());
        DConnectionResult res(new CConnectionResult(protocol, EConnectionStatus::CONNECTION_ERROR));
        resultsQueue.push(res);
      }
    }
  }

  bool CHttpThread::convertToBinary(QByteArray& array, const CProtocol& protocol)
  {

    array.reserve(protocol.getSize());
    convertToBinary(array, protocol.getVersion());
    convertToBinary(array, protocol.getSize());
    convertToBinary(array, protocol.getIdConcentrator());
    convertToBinary(array, protocol.getIdPackage());
    convertToBinary(array, protocol.getType());

    switch(protocol.getType()){
    case MONITOR_DATA:
      convertToBinary(array, std::dynamic_pointer_cast<CMonitorData>(protocol.getMessage()));
      break;

    case CONFIGURATION_RESPONSE:
      convertToBinary(array, std::dynamic_pointer_cast<CConfigurationResponse>(protocol.getMessage()));
      break;

    case SERVER_REQUEST:
      convertToBinary(array, std::dynamic_pointer_cast<CServerRequest>(protocol.getMessage()));
      break;
    case SERVER_MONITOR_RESPONSE:
      // typ protokolu server -> concentrator
      return false;
      break;
    }
    convertToBinary(array, protocol.getCRC());


    return true;
  }

  void CHttpThread::convertToBinary(QByteArray& array, const std::shared_ptr<CServerRequest>& reqs)
  {
    convertToBinary(array, reqs->getRequestsSize());
    std::for_each(reqs->getRequests().begin(), reqs->getRequests().end(),
                  [&](const CRequest& req){
                    convertToBinary(array, req);
                  });

  }

  void CHttpThread::convertToBinary(QByteArray& array, const CRequest& req)
  {
    convertToBinary(array, req.getIdSensor());
    convertToBinary(array, req.getConfigurationType());

  }

  void CHttpThread::convertToBinary(QByteArray& array,
                                    const std::shared_ptr<CConfigurationResponse>& confResp)
  {
    convertToBinary(array, confResp->getStatus());
    convertToBinary(array, confResp->getIdRequestPackage());
    convertToBinary(array, confResp->getCurrentConfiguration());
  }

  void CHttpThread::convertToBinary(QByteArray& array, const CConfiguration& conf)
  {
    convertToBinary(array, conf.getConfigurationsSize());
    std::for_each(conf.getConfigurations().begin(), conf.getConfigurations().end(),
                  [&](const CConfigurationValue& value){
                    convertToBinary(array, value);
                  });


  }

  void CHttpThread::convertToBinary(QByteArray& array, const CConfigurationValue& confValue)
  {
    convertToBinary(array, confValue.getIdSensor());
    convertToBinary(array, confValue.getConfigurationType());
    convertToBinary(array, confValue.getData());

  }

  template<typename T>
  void CHttpThread::convertToBinary(QByteArray& array, const T& data){
    array.append((const char*)&data, sizeof(data));
  }

  void CHttpThread::convertToBinary(QByteArray& array, const std::shared_ptr<CMonitorData>& monitorData)
  {
    convertToBinary(array, monitorData->getSendTime());
    convertToBinary(array, monitorData->getSensorAmount());
    convertToBinary(array, monitorData->getSensorsDataSize());

    std::for_each(monitorData->getSensorsData().begin(), monitorData->getSensorsData().end(),
                  [&](const CSensorData& value){
                    convertToBinary(array, value);
                  });


  }

  void CHttpThread::convertToBinary(QByteArray& array, const CSensorData& sensorData)
  {

    convertToBinary(array, sensorData.getIdData());
    convertToBinary(array, sensorData.getIdSensor());
    convertToBinary(array, sensorData.getTimeStamp());
    convertToBinary(array, sensorData.getSensorState());
    convertToBinary(array, sensorData.getDangerLevel());

    convertToBinary(array, sensorData.getData());
  }

  void CHttpThread::convertToBinary(QByteArray& array, const CData& data)
  {
    convertToBinary(array, data.getType());

    switch(data.getType()){
    case EValueType::INT_8:
      convertToBinary(array, data.getValue().vInt8);
      break;
    case EValueType::UINT_8:
      convertToBinary(array, data.getValue().vUInt8);
      break;
    case EValueType::INT_16:
      convertToBinary(array, data.getValue().vInt16);
      break;
    case EValueType::UINT_16:
      convertToBinary(array, data.getValue().vUInt16);
      break;
    case EValueType::INT_32:
      convertToBinary(array, data.getValue().vInt32);
      break;
    case EValueType::UINT_32:
      convertToBinary(array, data.getValue().vUInt32);
      break;
    case EValueType::INT_64:
      convertToBinary(array, data.getValue().vInt64);
      break;
    case EValueType::UINT_64:
      convertToBinary(array, data.getValue().vUInt64);
      break;
    case EValueType::FLOAT_32:
      convertToBinary(array, data.getValue().vFloat32);
      break;
    case EValueType::DOUBLE_64:
      convertToBinary(array, data.getValue().vDouble64);
      break;
    case EValueType::VOID:
      convertToBinary(array, data.getValue().vVoid8);
      break;
    }

  }
  // ///////////////////////// CONVERT TO PROTOCOL ////////////////////////

  std::shared_ptr<CProtocol> CHttpThread::convertToProtocol(CByteWrapper& wrapper){
    std::shared_ptr<CProtocol> protocol;

    uint8_t version;
    uint32_t size;
    uint16_t idConcentrator;
    uint32_t idPackage;
    EMessageType type;

    READ_WRAPPER(version, wrapper);
    if (version != VERSION)
    {
      return protocol;
    }
    READ_WRAPPER(size, wrapper);
    READ_WRAPPER(idConcentrator, wrapper);
    if (idConcentrator != NEngine::CConfigurationFactory::getInstance()->getIdConcentrator())
    {
      return protocol;
    }
    READ_WRAPPER(idPackage, wrapper);
    READ_WRAPPER(type, wrapper);

    std::shared_ptr<IMessage> message;
    if (!convertToProtocol(message, wrapper))
    {
      return protocol;
    }
    protocol.reset(new CProtocol(version, size, idConcentrator, idPackage, type, message));
    return protocol;
  }

  bool CHttpThread::convertToProtocol(std::shared_ptr<IMessage>& message, CByteWrapper& wrapper)
  {
    EReceiveStatus status;
    uint32_t idRequestPackage;

    READ_WRAPPER(status, wrapper);
    READ_WRAPPER(idRequestPackage, wrapper);

    std::vector<CConfigurationValue> configurations;

    if (convertToProtocol(configurations, wrapper))
    {
       CConfiguration configuration(configurations);
        message.reset(new CServerResponse(status, idRequestPackage, configuration));
        return true;
    }
    return false;
  }

//////////
  bool CHttpThread::convertToProtocol(std::vector<CConfigurationValue>& configurations, CByteWrapper& wrapper)
  {
    uint8_t configurationSize;
    READ_WRAPPER(configurationSize, wrapper);

    for(decltype(configurationSize) i = 0; i < configurationSize; ++i){
      int8_t idSensor;
      EConfigurationType configurationType;
      READ_WRAPPER(idSensor, wrapper);
      READ_WRAPPER(configurationType, wrapper);
      CData data;
      if (convertToProtocol(data, wrapper))
      {
        configurations.emplace_back(idSensor, configurationType, data);
      }
      else
      {
        // niepoprawny format
        return false;
      }
    }
    return true;
  }
////////////////
  bool CHttpThread::convertToProtocol(CData& sdata, CByteWrapper& wrapper)
  {
    EValueType type;
    READ_WRAPPER(type, wrapper);
    void* value;
    switch(type){
    case EValueType::INT_8:
      int8_t vInt8;
      READ_WRAPPER(vInt8, wrapper);
      value = &vInt8;
      break;
    case EValueType::UINT_8:
      uint8_t vUInt8;
      READ_WRAPPER(vUInt8, wrapper);
      value = &vUInt8;
      break;
    case EValueType::INT_16:
      int16_t vInt16;
      READ_WRAPPER(vInt16, wrapper);
      value = &vInt16;
      break;
    case EValueType::UINT_16:
      uint16_t vUInt16;
      READ_WRAPPER(vUInt16, wrapper);
      value = &vUInt16;
      break;
    case EValueType::INT_32:
      int32_t vInt32;
      READ_WRAPPER(vInt32, wrapper);
      value = &vInt32;
      break;
    case EValueType::UINT_32:
      uint32_t vUInt32;
      READ_WRAPPER(vUInt32, wrapper);
      value = &vUInt32;
      break;
    case EValueType::INT_64:
      int64_t vInt64;
      READ_WRAPPER(vInt64, wrapper);
      value = &vInt64;
      break;
    case EValueType::UINT_64:
      uint64_t vUInt64;
      READ_WRAPPER(vUInt64, wrapper);
      value = &vUInt64;
      break;
    case EValueType::FLOAT_32:
      float32_t vFloat32;
      READ_WRAPPER(vFloat32, wrapper);
      value = &vFloat32;
      break;
    case EValueType::DOUBLE_64:
      double64_t vDouble64;
      READ_WRAPPER(vDouble64, wrapper);
      value = &vDouble64;
      break;
    case EValueType::VOID:
      uint8_t vVoid8;
      READ_WRAPPER(vVoid8, wrapper);
      value = &vVoid8;
      break;
    default:
      return false;
    }
    sdata.setValue(type, value);
    return true;
  }
}
