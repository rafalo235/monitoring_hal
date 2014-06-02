#ifndef LOGGER_H
#define LOGGER_H

#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <algorithm>
#include <QtCore>
#include <memory>

#define LOGGER_ENABLE

#ifdef LOGGER_FILE_ENABLE
#include <fstream>
#include <map>
#endif

#ifdef LOGGER_ENABLE
#include "communication/interfaces/protocol.h"
#endif

// konwersja pliku z nazwa, do samej nazwy - slash zalezy od OS'a
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
      #define __FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
      #define __FILE_NAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif




#ifdef LOGGER_ENABLE

#define LOG_DEBUG(...) NUtil::CLogger::getInstance()->logDebug(__FILE_NAME__, __FUNCTION__, __LINE__, __VA_ARGS__);

// wylaczenie logowania
// unikatowe nazwy
#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)
#define UNIQUE_NAME COMBINE(x,__LINE__) (__FILE_NAME__);
// makro wylaczajace debug log w pliku
#define LOG_OFF NUtil::CUnlogFile UNIQUE_NAME;

// makro do logowania protokolu
#define LOG_PROTOCOL(prot) NUtil::CLogger::getInstance()->logProtocol(prot);

#else // LOGGER_ENABLE
// bez logowania debugow
#define LOG_DEBUG(...)
#define LOG_PROTOCOL(prot)
#endif // LOGGER_ENABLE

#define LOG_ERROR(...) NUtil::CLogger::getInstance()->logError(__FILE_NAME__, __FUNCTION__, __LINE__, __VA_ARGS__);


//! \brief Logger do prostychy logow (do pliku tez).
//! Wlaczenie logowania nastepuje po zdefinioawniu LOGGER_ENABLE. Wlacznie logowania do pliku LOGGER_FILE_ENABLE.
//! Logi typu Error sa zawsze logowane. Liczba parametrow jest dowolna.
//! Przyklady:
//!   LOG_DEBUG("Jakis sobie log debug z parametrami", 12, -23.0f);
//!   LOG_ERROR("Jakis sobie log error z parametrami", 2);
//!
//! Zatem logowanie odbywac sie tylko i wylaczenie przez makra, a nie przez klase.
//! Klasa sama w sobie jest singletonem, aby poprawnie zamknac plik logow.
//! LOG_OFF - wylacza debugowanie logow dla danego pliku
//! \author M. Serwach
//!
namespace NUtil{
#ifdef LOGGER_ENABLE

  class CUnlogFile{
    static std::set<std::string> turnOffFiles;

  public:
    CUnlogFile(const char* file){
      turnOffFiles.insert(std::string(file));
    }

    static bool find(const char* file){

      return turnOffFiles.find(std::string(file)) != turnOffFiles.end();
    }
  };

#endif // LOGGER_ENABLE

  class CLogger{

  private:
    // singleton, wiec zabraniamy tworzyc konstruktora kopiujacego i operator przypisania
    CLogger() = default;
    CLogger(const CLogger&) = delete;
    CLogger& operator=(const CLogger&) = delete;

    static std::shared_ptr<CLogger> logger;


#ifdef LOGGER_FILE_ENABLE
    std::fstream file;
#endif

  public:
    static std::shared_ptr<CLogger> getInstance(){
      if (logger.use_count() == 0){
        logger.reset(new CLogger());
        // otworz plik jesli trzeba
#ifdef LOGGER_FILE_ENABLE
        logger->file.open("log.txt", std::ios::out);
#endif
#ifdef LOGGER_ENABLE
        logger->initEnums();
#endif
      }
      return logger;
    }

    // destruktor jest potrzebny tylko wtedy, gdy jest logowanie do pliku.
#ifdef LOGGER_FILE_ENABLE
    ~CLogger(){
      file.close();
    }
#endif
    // ///////////////////// loggery ///

  private:

    template<typename streamType, typename Arg>
    void logTemplate(streamType& stream, Arg arg)
    {
      stream << arg << std::endl;

    }

    template<typename streamType, typename Type, typename ... Args> // typenamne... oznacza zmienna ilosc parametrow
    void logTemplate(streamType& stream, Type arg0, Args ... args)
    {
      stream << arg0 << " ";
      logTemplate(stream, args...); // po prawej ... oznacza rozpakowanie parametrow template'a

    }

    // //////////////////////////////// public /////////////////////////////////


    template<typename streamType, typename ... Args>
    void log(streamType& stream, const char* file, const char* function, const int line,  Args ... args)
    {

      stream<<"FILE: "<<file<<" FUNCTION: "<<function<<" LINE: "<<line<<" THREAD ID: "<<QThread::currentThreadId()<<std::endl;
      logTemplate(stream, args...);

#ifdef LOGGER_FILE_ENABLE
      this->file<<"FILE: "<<file<<" FUNCTION: "<<function<<" LINE: "<<line<<std::endl;
      logTemplate(this->file, args...);
      this->file.flush();
#endif
    }

  public:

#ifdef LOGGER_ENABLE

    template<typename ... Args>
    void logDebug(const char* file, const char* function, const int line,  Args ... args)
    {

      if (!CUnlogFile::find(file)){
        log(std::cout, file, function, line, args...);
      }
    }
#endif

    template<typename ... Args>
    void logError(const char* file, const char* function, const int line,  Args ... args)
    {
      log(std::cerr, file, function, line, args...);
    }


#ifdef LOGGER_ENABLE
  private:
    static std::map<NProtocol::ESensorState, std::string> sensorStateEnums;
    static std::map<NProtocol::EDangerLevel, std::string> dangerLevelEnums;
    static std::map<NProtocol::EValueType, std::string> valueTypeEnums;
    static std::map<NProtocol::EConfigurationType, std::string> configurationTypeEnums;
    static std::map<NProtocol::EMessageType, std::string> messageTypeEnums;
    static std::map<NProtocol::EReceiveStatus, std::string> receiveStatusEnums;


    void initEnums(){
      valueTypeEnums[NProtocol::EValueType::INT_8] = "INT_8";
      valueTypeEnums[NProtocol::EValueType::UINT_8] = "UINT_8";
      valueTypeEnums[NProtocol::EValueType::INT_16] = "UINT_16";
      valueTypeEnums[NProtocol::EValueType::UINT_16] = "UINT_16";
      valueTypeEnums[NProtocol::EValueType::INT_32] = "INT_32";
      valueTypeEnums[NProtocol::EValueType::UINT_32] = "UINT_32";
      valueTypeEnums[NProtocol::EValueType::INT_64] = "INT_64";
      valueTypeEnums[NProtocol::EValueType::UINT_64] = "UINT_64";
      valueTypeEnums[NProtocol::EValueType::FLOAT_32] = "FLOAT_32";
      valueTypeEnums[NProtocol::EValueType::DOUBLE_64] = "DOUBLE_64";
      valueTypeEnums[NProtocol::EValueType::VOID] = "VOID";

      configurationTypeEnums[NProtocol::EConfigurationType::SENDING_FREQUENCY] = "SENDING_FREQUENCY";
      configurationTypeEnums[NProtocol::EConfigurationType::SENSOR_TURN_ON] = "SENSOR_TURN_ON";
      configurationTypeEnums[NProtocol::EConfigurationType::CARD_SAVE] = "CARD_SAVE";
      configurationTypeEnums[NProtocol::EConfigurationType::DANGER_LEVEL] = "DANGER_LEVEL";
      configurationTypeEnums[NProtocol::EConfigurationType::ALARM_LEVEL] = "ALARM_LEVEL";
      configurationTypeEnums[NProtocol::EConfigurationType::RESET] = "RESET";
      configurationTypeEnums[NProtocol::EConfigurationType::KEY_CHANGE] = "KEY_CHANGE";
      configurationTypeEnums[NProtocol::EConfigurationType::KEY_LIFETIME] = "KEY_LIFETIME";

      messageTypeEnums[NProtocol::EMessageType::MONITOR_DATA] = "MONITOR_DATA";
      messageTypeEnums[NProtocol::EMessageType::CONFIGURATION_RESPONSE] = "CONFIGURATION_RESPONSE";
      messageTypeEnums[NProtocol::EMessageType::SERVER_REQUEST] = "SERVER_REQUEST";
      messageTypeEnums[NProtocol::EMessageType::SERVER_MONITOR_RESPONSE] = "SERVER_MONITOR_RESPONSE";

      sensorStateEnums[NProtocol::ESensorState::OK] = "OK";
      sensorStateEnums[NProtocol::ESensorState::TURN_OFF] = "TURN_OFF";
      sensorStateEnums[NProtocol::ESensorState::BROKEN] = "BROKEN";

      dangerLevelEnums[NProtocol::EDangerLevel::NONE] = "NONE";
      dangerLevelEnums[NProtocol::EDangerLevel::WARNING] = "WARNING";
      dangerLevelEnums[NProtocol::EDangerLevel::ALARM] = "ALARM";


      receiveStatusEnums[NProtocol::EReceiveStatus::OK] = "OK";
      receiveStatusEnums[NProtocol::EReceiveStatus::CRC_ERROR] = "CRC_ERROR";
      receiveStatusEnums[NProtocol::EReceiveStatus::BAD_STRUCTURE] = "BAD_STRUCTURE";
      receiveStatusEnums[NProtocol::EReceiveStatus::OPERATION_FAILED] = "OPERATION_FAILED";

    }

    static void logProtocol(const NProtocol::SData& sdata)
    {
      std::cout<<"sdata value: ";
      switch(sdata.type){
      case NProtocol::EValueType::INT_8:
        std::cout<<sdata.value.vInt8;
        break;
      case NProtocol::EValueType::UINT_8:
        std::cout<<sdata.value.vUInt8;
        break;
      case NProtocol::EValueType::INT_16:
        std::cout<<sdata.value.vInt16;
        break;
      case NProtocol::EValueType::UINT_16:
        std::cout<<sdata.value.vUInt16;
        break;
      case NProtocol::EValueType::INT_32:
        std::cout<<sdata.value.vInt32;
        break;
      case NProtocol::EValueType::UINT_32:
        std::cout<<sdata.value.vUInt32;
        break;
      case NProtocol::EValueType::INT_64:
        std::cout<<sdata.value.vInt64;
        break;
      case NProtocol::EValueType::UINT_64:
        std::cout<<sdata.value.vUInt64;
        break;
      case NProtocol::EValueType::FLOAT_32:
        std::cout<<sdata.value.vFloat32;
        break;
      case NProtocol::EValueType::DOUBLE_64:
        std::cout<<sdata.value.vDouble64;
        break;
      case NProtocol::EValueType::VOID:
        std::cout<<sdata.value.vVoid8;
        break;
      }
      std::cout<<" ("<<valueTypeEnums[sdata.type]<<")"<<std::endl;
    }

    static void logProtocol(const NProtocol::SSensorData& sensorsData)
    {
      std::cout<<"idData: "<<sensorsData.idData<<std::endl;
      std::cout<<"idSensor: "<<static_cast<int>(sensorsData.idSensor)<<std::endl;
      std::cout<<"timeStamp: "<<sensorsData.timeStamp<<std::endl;
      std::cout<<"sensorState: "<<sensorStateEnums[sensorsData.sensorState]<<std::endl;
      std::cout<<"dangerLevel: "<<dangerLevelEnums[sensorsData.dangerLevel]<<std::endl;

    }

    static void logProtocol(const NProtocol::SRequest& requests)
    {
      std::cout<<"idSensor: "<<static_cast<int>(requests.idSensor)<<std::endl;
      std::cout<<"configurationType: "<<configurationTypeEnums[requests.configurationType]<<std::endl;

    }

    static void logProtocol(const NProtocol::SMonitorData& monitorData)
    {
      std::cout<<"sendTime: "<<monitorData.sendTime<<std::endl;
      std::cout<<"sensorsAmount: "<<static_cast<int>(monitorData.sensorsAmount)<<std::endl;
      std::cout<<"sensorsDataSize: "<<monitorData.sensorsDataSize<<std::endl;
      std::for_each(monitorData.sensorsData, monitorData.sensorsData + monitorData.sensorsDataSize,
                    [&](const NProtocol::SSensorData& sensorsData){ logProtocol(sensorsData);} );
    }

    static void logProtocol(const NProtocol::SConfigurationValue& confValue)
    {
      std::cout<<"idSensor: "<<static_cast<int>(confValue.idSensor)<<std::endl;
      std::cout<<"configurationType: "<<configurationTypeEnums[confValue.configurationType]<<std::endl;
      logProtocol(confValue.data);
    }

    static void logProtocol(const NProtocol::SConfiguration& configuration)
    {
      std::cout<<"configurationSize: "<<static_cast<int>(configuration.configurationSize)<<std::endl;
      std::for_each(configuration.configurations, configuration.configurations + configuration.configurationSize,
                    [&](const NProtocol::SConfigurationValue& value){ logProtocol(value);} );
    }

    static void logProtocol(const NProtocol::SConfigurationResponse& configurationResponse)
    {
      std::cout<<"status: "<<receiveStatusEnums[configurationResponse.status]<<std::endl;
      std::cout<<"idRequestPackage: "<<configurationResponse.idRequestPackage<<std::endl;
      logProtocol(configurationResponse.currentConfiguration);
    }

    static void logProtocol(const NProtocol::SServerRequest& serverRequest)
    {
      std::cout<<"requestsSize: "<<static_cast<int>(serverRequest.requestsSize)<<std::endl;
      std::for_each(serverRequest.requests, serverRequest.requests + serverRequest.requestsSize,
                    [&](const NProtocol::SRequest& req){ logProtocol(req);} );
    }

    static void logProtocol(const NProtocol::SServerResponse& serverMonitorResponse)
    {
      std::cout<<"status: "<<receiveStatusEnums[serverMonitorResponse.status]<<std::endl;
      std::cout<<"idRequestPackage: "<<serverMonitorResponse.idRequestPackage<<std::endl;
      logProtocol(serverMonitorResponse.configuration);
    }

  public:
    void logProtocol(const NProtocol::SProtocol& protocol)
    {
      std::cout<<"version: "<<static_cast<int>(protocol.version)<<std::endl;
      std::cout<<"size: "<<protocol.size<<std::endl;
      std::cout<<"idConcentrator: "<<protocol.idConcentrator<<std::endl;
      std::cout<<"idPackage: "<<protocol.idPackage<<std::endl;
      std::cout<<"type: "<<messageTypeEnums[protocol.type]<<std::endl;
      switch(protocol.type){
      case NProtocol::EMessageType::MONITOR_DATA:
        logProtocol(protocol.message.monitorData);
        break;
      case NProtocol::EMessageType::CONFIGURATION_RESPONSE:
        logProtocol(protocol.message.configurationResponse);
        break;
      case NProtocol::EMessageType::SERVER_MONITOR_RESPONSE:
        logProtocol(protocol.message.serverMonitorResponse);
        break;
      case NProtocol::EMessageType::SERVER_REQUEST:
        logProtocol(protocol.message.serverRequest);
        break;
      }
    }

#endif
  };
}

// ////////////////////////////////////////////////////////////


#endif // LOGGER_H
