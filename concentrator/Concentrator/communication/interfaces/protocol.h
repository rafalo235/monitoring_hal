/*
 * protocol.h
 *
 * \brief Plik naglowkowy protokolu komunikacji pomiedzy serwerem a koncentratorem systemu monitorowania hal.
 * \author: Marcin Serwach
 * \version 1.0
 * \date 24.04.2014
 * \details
 *      C++ standard: C++11
 *
 *      Zastosowano przedrostki:
 *      E - enum
 *      N - namespace
 *      S - struct
 *
 *      Formatowanie: doxygen
 *
 *      Zalozenia:
 *      1) sizeof(float) = 4
 *      2) sizeof(double) = 8
 *      3) sizeof(long long) = 8
 *      4) Kolejnosc bajtow Little Endian (ewentualna konwersja powinna zostac przeprowadzona przed zapisem)
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_


#include <cstdint>
#include <algorithm>

namespace NProtocol
{
  //! \brief wesja protokolu
  const uint8_t VERSION = 1u;


  typedef float float32_t;    //!< 32 bitowy float
  typedef double double64_t;  //!< 64 bitowy double
  typedef unsigned long long uint64_t; //!< 64 bitowy int bez znaku - przenosnosc dla time_t

  //! \brief Typ danej przechowywanej w UValue
  enum class EValueType : int8_t
  {
    INT_8 = 0,      //!< UValue przechowuje int8_t
    UINT_8 = 1,     //!< UValue przechowuje uint8_t
    INT_16 = 2,     //!< UValue przechowuje int16_t
    UINT_16 = 3,    //!< UValue przechowuje uint16_t
    INT_32 = 4,     //!< UValue przechowuje int32_t
    UINT_32 = 5,    //!< UValue przechowuje uint32_t
    INT_64 = 6,     //!< UValue przechowuje int64_t
    UINT_64 = 7,    //!< UValue przechowuje uint64_t
    FLOAT_32 = 8,   //!< UValue przechowuje float32_t
    DOUBLE_64 = 9,  //!< UValue przechowuje double64_t
    VOID = 10       //!< UValue przechowuje nic nieznacz�ca wartosc cVoidValue
  };

  //! \brief Nic nieznaczaca dana wykorzystana dla typu EValueType::VOID
  const uint8_t cVoidValue = 0xFF;

  //! \brief Wartosci przechowywane w strukturze
  union UValue
  {
    int8_t vInt8;
    uint8_t vUInt8;
    int16_t vInt16;
    uint16_t vUInt16;
    int32_t vInt32;
    uint32_t vUInt32;
    int64_t vInt64;
    uint64_t vUInt64;
    float32_t vFloat32;
    double64_t vDouble64;
    uint8_t vVoid8;
  };

  //! \brief Struktrura przechowywujace wartosci
  struct SData
  {
    EValueType type;  //!< Typ wartosci przechowywanej w UValue
    UValue value;     //!< Wartosc o typie EValueType

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(type);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      switch(type){
      case EValueType::INT_8:
        return headerSize + sizeof(value.vInt8);
      case EValueType::UINT_8:
        return headerSize + sizeof(value.vUInt8);
      case EValueType::INT_16:
        return headerSize + sizeof(value.vInt16);
      case EValueType::UINT_16:
        return headerSize + sizeof(value.vUInt16);
      case EValueType::INT_32:
        return headerSize + sizeof(value.vInt32);
      case EValueType::UINT_32:
        return headerSize + sizeof(value.vUInt32);
      case EValueType::INT_64:
        return headerSize + sizeof(value.vInt64);
      case EValueType::UINT_64:
        return headerSize + sizeof(value.vUInt64);
      case EValueType::FLOAT_32:
        return headerSize + sizeof(value.vFloat32);
      case EValueType::DOUBLE_64:
        return headerSize + sizeof(value.vDouble64);
      case EValueType::VOID:
        return headerSize + sizeof(value.vVoid8);
      default:
        return 0;
      }

    }

    static SData getVoid(){
      SData s;
      s.type = EValueType::VOID;
      s.value.vVoid8 = cVoidValue;
      return s;
    }
  };

  //! \brief Opcja konfiguracji
  enum class EConfigurationType : int8_t
  {
    //! \brief Czestotliwosc wysylania danych
    SENDING_FREQUENCY = 0,
    //! \brief Wylaczenie/wlaczenie czujnika
    SENSOR_TURN_ON = 1,
    //! \brief Wylaczenie/wlaczenie zapisu na karte pamieci
    CARD_SAVE = 2,
    //! \brief Wartosc czujnika powodujaca poziom niebezpieczenstwa EDangerLevel = DANGEROUS
    DANGER_LEVEL = 3,
    //! \brief Wartosc czujnika powodujaca poziom niebezpieczenstwa EDangerLevel = ALARM
    ALARM_LEVEL = 4,
    //! \brief Resetowanie koncentratora
    RESET = 5,
    //! \brief zmiana klucza
    KEY_CHANGE = 6,
    //! \brief czas wygasniecia klucza
    KEY_LIFETIME = 7
  };

  //! \brief Typ wiadomosci przekazywanej przez pekiet. Definiuje wartosc UMessage
  enum EMessageType : int8_t
  {
    //! \brief Koncentrator przesyla cykliczne dane z czujnikow. UMessage przechowuje SMonitorData.
    MONITOR_DATA = 0,
    //! \brief Koncentrator potwierdza zmiane konfiguracji. UMessage przechowuje SConfigurationResponse.
    CONFIGURATION_RESPONSE = 1,
    //! \brief Koncentrator prosi o przeslanie konfiguracji. UMessage przechowuje SServerRequest.
    SERVER_REQUEST = 2,
    //! \brief Serwer potwierdza odebrane dane z czujnik z koncentrator
    //!        i ewentualnie przesyla konfiguracje do koncentratora na zadanie koncentratora lub serwera. UMessage przechowuje SServerMonitorResponse.
    SERVER_MONITOR_RESPONSE = 3,
  };

  //! \brief Status pojedynczego czujnika.
  enum class ESensorState : int8_t
  {
    OK = 0,       //!< czujnik dziala poprawnie
    TURN_OFF = 1, //!< czujnik wylaczony
    BROKEN = 2,   //!< czujnik nie dziala poprawnie
  };

  //! \brief Poziom niebezpieczenstwa nadany przez koncentrator.
  enum class EDangerLevel : int8_t
  {
    NONE = 0,       //!< dane z czujnika nie wskazuja zagrozenia.
    WARNING = 1,     //!< dane z czujnika wskazuja pierwszy stopien zagrozenia
    ALARM = 2       //!< dane z czujnika wskazuja niebezpieczenstwo
  };

  //! \brief Dane pojedynczego pomiaru.
  struct SSensorData
  {
    uint32_t idData;           //!< id pomiaru
    uint8_t idSensor;          //!< id czujnika
    uint64_t timeStamp;        //!< czas pomiaru
    ESensorState sensorState;  //!< stan czujnika
    EDangerLevel dangerLevel;  //!< stopien niebezpieczenstwa
    SData data; //!< dane pomiaru; dla sensorState != ESensorState.OK pole data przechowuje wartosc cVoidValue

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(idData) + sizeof(idSensor) +
                                        sizeof(timeStamp) +
                                       sizeof(sensorState) + sizeof(dangerLevel);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      return headerSize + data.getSize();
    }
  };

  //! \brief Dane z czujnikow wysylane z koncentrator do serwera
  struct SMonitorData
  {
    uint64_t sendTime;        //!< czas pierwszej proby wyslania pakietu
    uint8_t sensorsAmount;    //!< ilosc czujnikow
    uint32_t sensorsDataSize; //!< wielkosc tablicy SSensorData
    SSensorData* sensorsData; //!< tablica z danymi z czujnikow

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(sendTime) + sizeof(sensorsAmount) +
                                        sizeof(sensorsDataSize);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      uint32_t size = 0;
      std::for_each(sensorsData, sensorsData + sensorsDataSize,
                    [&size](SSensorData data){size += data.getSize();});
      return headerSize + size;
    }
  };

  //! \brief Id koncentratora
  const int8_t cIdConcentrator = 0xFF;

  //! \brief Pojedyncza wartosc konfigurowalna.
  struct SConfigurationValue
  {
    int8_t idSensor;        //!< id czujnika lub koncentratora (cIdConcentrator)
    EConfigurationType configurationType;  //!< opcja konfiguracji
    SData data;                            //!< wartosc dla danej opcji

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(idSensor) + sizeof(configurationType);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      return headerSize + data.getSize();
    }
  };

  //! \brief Ustawienia konfiguracyjne
  struct SConfiguration
  {
    uint8_t configurationSize;           //!< Rozmiar tablicy configurations
    SConfigurationValue* configurations; //!< Tablica opcji konfiguracji

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(configurationSize);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      uint32_t size = 0;
      std::for_each(configurations, configurations + configurationSize,
                    [&size](SConfigurationValue data){size += data.getSize();});
      return headerSize + size;
    }
  };

  //! \brief Status otrzymanego pakietu, ktory jest wysylany wraz z odpowiedzia
  enum class EReceiveStatus : int8_t
  {
    OK,               //!< Pakiet otrzymany byl poprawny
    CRC_ERROR,        //!< Pakiet otrzymany mial niepoprawna CRC
    BAD_STRUCTURE,    //!< Pakiet otrzymany zawieral niepoprawna strukture
    OPERATION_FAILED  //!< Zadanie przekazane przez otrzymany pakiet nie moglo zostac wykonane
  };

  //! \brief Potwierdzenie zmiany konfiguracji koncentratora do serwera
  struct SConfigurationResponse
  {
    EReceiveStatus status;               //!< Status otrzymanego pakietu konfiguracyjnego lub status zmiany konfiguracji
    uint32_t idRequestPackage;           //!< ID pakietu, ktory zglosil zmiane konfiguracji - pole SProtocol.idPackage
    SConfiguration currentConfiguration; //!< Konfiguracja koncentratora

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(status) + sizeof(idRequestPackage);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      return headerSize + currentConfiguration.getSize();
    }
  };

  //! \brief Pojedyncze zadanie opcji konfigurowalnej
  struct SRequest
  {
    int8_t idSensor;                      //!< id czujnika lub koncentratora (cIdConcentrator)
    EConfigurationType configurationType; //!< opcja konfiguracji

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(idSensor) + sizeof(configurationType);
  };

  //! Prosba koncentratora o wyslanie konfiguracji
  struct SServerRequest
  {
    uint8_t requestsSize; //!< rozmiar tablicy requests
    SRequest* requests;   //!< tablica o wielkosci requestsSize zadan konfiguracji

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(requestsSize);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      return headerSize + SRequest::headerSize * requestsSize;
    }
  };

  //! \brief Odpowiedz serwera po otrzymaniu danych z czujnikow.
  struct SServerResponse
  {
    EReceiveStatus status;        //!< Status otrzymanego pakietu z danymi z czujnikow
    uint32_t idRequestPackage;    //!< ID pakietu, ktory wyslal dane z czujnikow
    SConfiguration configuration; //!< Zadanie zmiany konfiguracji koncentratora

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize = sizeof(status) + sizeof(idRequestPackage);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      return headerSize + configuration.getSize();
    }
  };

  //! \brief Wiadomosc przesylana protokolem.
  union UMessage
  {
    //! \brief Dane z czujnikow (SProtocol.type = EMessageType .MONITOR_DATA)
    SMonitorData monitorData;
    //! \brief Potwierdzenie konfiguracji koncentratora (SProtocol.type = EMessageType.CONFIGURATION_RESPONSE)
    SConfigurationResponse configurationResponse;
    //! \brief Prosba koncentratora o przeslanie konfiguracji (SProtocol.type = EMessageType.SERVER_REQUEST)
    SServerRequest serverRequest;
    //! \brief Potwierdzenie serwera otrzymanych danych i ewentualne wyslanie konfiguracji (SProtocol.type = EMessageType.SERVER_RESPONSE)
    SServerResponse serverMonitorResponse;
  };

  //! \brief Struktura protokolu.
  struct SProtocol
  {
    //! \brief Wersja protkolu
    uint8_t version;
    //! \brief Rozmiar laczny danych przesylanych protokolem.
    uint32_t size;
    //! \brief ID koncentratora
    uint16_t idConcentrator;
    //! \brief ID danych pakietu danych.
    uint32_t idPackage;
    //! \brief Typ pakietu danych wysylanych protokolem
    EMessageType type;
    //! \brief Dane wysylane protokolem. Typ danych zalezny do wartoci idPackage.
    UMessage message;
    //! \brief Cykliczny kod nadmiarowy dla calej struktury
    uint16_t crc;

    const static uint32_t headerSize = sizeof(version) + sizeof(size) +
                                        sizeof(idConcentrator) + sizeof(idPackage) + sizeof(type) + sizeof(crc);

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const{
      switch(type){
      case MONITOR_DATA:
        return headerSize + message.monitorData.getSize();
        break;
      case CONFIGURATION_RESPONSE:
        return headerSize + message.configurationResponse.getSize();
        break;
      case SERVER_MONITOR_RESPONSE:
        return headerSize + message.serverMonitorResponse.getSize();
        break;
      case SERVER_REQUEST:
        return headerSize + message.serverRequest.getSize();
        break;
      default:
        return 0;
      }

    }
  };


}

#endif /* PROTOCOL_H_ */
