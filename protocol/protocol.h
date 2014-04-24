/*
 * protocol.h
 *
 * \brief Plik naglowkowy protokolu komunikacji pomiedzy serwerem a koncentratorem systemu monitorowania hal.
 * \author: Marcin Serwach
 * \version 1.0
 * \date 24.04.2014
 * \details
 *      C++ standard: C++03
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

//! \brief wersja protokolu
#define PROTOCOL_VERSION 1

#include <cstdint>

namespace NProtocol
{

  typedef float float32_t;    //!< 32 bitowy float
  typedef double double64_t;  //!< 64 bitowy double
  typedef unsigned long long uint64_t; //!< 64 bitowy int bez znaku - przenosnosc dla time_t

  //! \brief Typ danej przechowywanej w UValue
  enum EValueType
  {
    INT_8,      //!< UValue przechowuje int8_t
    UINT_8,     //!< UValue przechowuje uint8_t
    INT_16,     //!< UValue przechowuje int16_t
    UINT_16,    //!< UValue przechowuje uint16_t
    INT_32,     //!< UValue przechowuje int32_t
    UINT_32,    //!< UValue przechowuje uint32_t
    INT_64,     //!< UValue przechowuje int64_t
    UINT_64,    //!< UValue przechowuje uint64_t
    FLOAT_32,   //!< UValue przechowuje float32_t
    DOUBLE_64,  //!< UValue przechowuje double64_t
    VOID        //!< UValue przechowuje nic nieznacz¹ca wartosc cVoidValue
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
  };

  //! \brief Opcja konfiguracji
  enum EConfigurationType
  {
    //! \brief Czestotliwosc wysylania danych
    SENDING_FREQUENCY,
    //! \brief Wylaczenie/wlaczenie czujnika
    SENSOR_TURN_ON,
    //! \brief Wylaczenie/wlaczenie zapisu na karte pamieci
    CARD_SAVE,
    //! \brief Wartosc czujnika powodujaca poziom niebezpieczenstwa EDangerLevel = DANGEROUS
    DANGER_LEVEL,
    //! \brief Wartosc czujnika powodujaca poziom niebezpieczenstwa EDangerLevel = ALARM
    ALARM_LEVEL,
    //! \brief Resetowanie koncentratora
    RESET,
    //! \brief zmiana klucza
    KEY_CHANGE,
    //! \brief czas wygasniecia klucza
    KEY_LIFETIME
  };

  //! \brief Typ wiadomosci przekazywanej przez pekiet. Definiuje wartosc UMessage
  enum EMessageType
  {
    //! \brief Koncentrator przesyla cykliczne dane z czujnikow. UMessage przechowuje SMonitorData.
    MONITOR_DATA,
    //! \brief Koncentrator potwierdza zmiane konfiguracji. UMessage przechowuje SConfigurationResponse.
    CONFIGURATION_RESPONSE,
    //! \brief Koncentrator prosi o przeslanie konfiguracji. UMessage przechowuje SServerRequest.
    SERVER_REQUEST,
    //! \brief Serwer potwierdza odebrane dane z czujnik z koncentrator
    //!        i ewentualnie przesyla konfiguracje do koncentratora na ¿adanie koncentratora lub serwera. UMessage przechowuje SServerMonitorResponse.
    SERVER_MONITOR_RESPONSE,
  };

  //! \brief Status pojedynczego czujnika.
  enum ESensorState
  {
    OK,       //!< czujnik dziala poprawnie
    TURN_OFF, //!< czujnik wylaczony
    BROKEN,   //!< czujnik nie dziala poprawnie
  };

  //! \brief Poziom niebezpieczenstwa nadany przez koncentrator.
  enum EDangerLevel
  {
    NONE,       //!< dane z czujnika nie wskazuja zagrozenia.
    DANGER,     //!< dane z czujnika wskazuja pierwszy stopien zagrozenia
    ALARM       //!< dane z czujnika wskazuja niebezpieczenstwo
  };

  //! \brief Dane pojedynczego pomiaru.
  struct SSensorData
  {
    uint32_t idData;           //!< id pomiaru
    uint8_t idSensor;          //!< id czujnika
    uint64_t timeStamp;        //!< czas pomiaru
    ESensorState sensorState;  //!< stan czujnika
    EDangerLevel dataState;    //!< stopien niebezpieczenstwa
    SData data; //!< dane pomiaru; dla sensorState != ESensorState.OK pole data przechowuje wartosc cVoidValue
  };

  //! \brief Dane z czujnikow wysylane z koncentrator do serwera
  struct SMonitorData
  {
    uint64_t sendTime;        //!< czas pierwszej proby wyslania pakietu
    uint8_t sensorsAmount;    //!< ilosc czujnikow
    uint32_t sensorsDataSize; //!< wielkosc tablicy SSensorData
    SSensorData* sensorsData; //!< tablica z danymi z czujnikow
  };

  //! \brief Id koncentratora
  const int8_t cIdConcentrator = 0xFF;

  //! \brief Pojedyncza wartosc konfigurowalna.
  struct SConfigurationValue
  {
    int8_t idSensor;        //!< id czujnika lub koncentratora (cIdConcentrator)
    EConfigurationType configurationType;  //!< opcja konfiguracji
    SData data;                            //!< wartosc dla danej opcji
  };

  //! \brief Ustawienia konfiguracyjne
  struct SConfiguration
  {
    uint8_t configurationSize;           //!< Rozmiar tablicy configurations
    SConfigurationValue* configurations; //!< Tablica opcji konfiguracji
  };

  //! \brief Status otrzymanego pakietu, ktory jest wysylany wraz z odpowiedzia
  enum EReceiveStatus
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
  };

  //! \brief Pojedyncze zadanie opcji konfigurowalnej
  struct SRequest
  {
    int8_t idSensor;                      //!< id czujnika lub koncentratora (cIdConcentrator)
    EConfigurationType configurationType; //!< opcja konfiguracji
  };

  //! Prosba koncentratora o wyslanie konfiguracji
  struct SServerRequest
  {
    uint8_t requestsSize; //!< rozmiar tablicy requests
    SRequest* requests;   //!< tablica o wielkosci requestsSize zadan konfiguracji
  };

  //! \brief Odpowiedz serwera po otrzymaniu danych z czujnikow.
  struct SServerResponse
  {
    EReceiveStatus status;        //!< Status otrzymanego pakietu z danymi z czujnikow
    uint32_t idRequestPackage;    //!< ID pakietu, ktory wyslal dane z czujnikow
    SConfiguration configuration; //!< Zadanie zmiany konfiguracji koncentratora
  };

  //! \brief Wiadomosc przesylana protokolem.
  union UMessage
  {
    //! \brief Dane z czujnikow (SProtocol.type = EMessageType .MONITOR_DATA)
    SMonitorData monitorData;
    //! \brief Potwierdzenie konfiguracji koncentratora (SProtocol.type = EMessageType.CONFIGURATION_RESPONSE)
    SConfigurationResponse configurationResponse;
    //! \brief Prosba koncentratora o przeslanie konfiguracji (SProtocol.type = EMessageType.SERVER_REQUEST)
    SServerRequest serverResponse;
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
    //! \brief Cykliczny kod nadmiarowy dla calej struktury
    uint16_t crc;
    //! \brief ID danych pakietu danych.
    uint32_t idPackage;
    //! \brief Typ pakietu danych wysylanych protokolem
    EMessageType type;
    //! \brief Dane wysylane protokolem. Typ danych zalezny do wartoci idPackage.
    UMessage message;
  };

}

#endif /* PROTOCOL_H_ */
