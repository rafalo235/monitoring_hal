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
 *      C - class
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
#include <vector>
#include <memory>

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
  class CData
  {
    EValueType type;  //!< Typ wartosci przechowywanej w UValue
    UValue value;     //!< Wartosc o typie EValueType

  public:

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    CData() noexcept {
      value.vVoid8 = cVoidValue;
      type = EValueType::VOID;
    }

    //!
    //! \brief SData Konstruktor nadajacy wartosc
    //! \param type1 typ wartosci
    //! \param value1 wskaznik do wartosci
    //!
    CData(const EValueType& type1, void* value1) noexcept
    {
      setValue(type1, value1);
    }

    CData(const CData&) = default;
    CData& operator=(const CData&) = default;

    void setValue(const EValueType& type1, void* value1) noexcept{
      type = type1;
      switch(type){
      case EValueType::INT_8:
        value.vInt8 = *(reinterpret_cast<int8_t*>(value1));
        break;
      case EValueType::UINT_8:
        value.vUInt8 = *(reinterpret_cast<uint8_t*>(value1));
        break;
      case EValueType::INT_16:
        value.vInt16 = *(reinterpret_cast<int16_t*>(value1));
        break;
      case EValueType::UINT_16:
        value.vUInt16 = *(reinterpret_cast<uint16_t*>(value1));
        break;
      case EValueType::INT_32:
        value.vInt32 = *(reinterpret_cast<int32_t*>(value1));
        break;
      case EValueType::UINT_32:
        value.vUInt32 = *(reinterpret_cast<uint32_t*>(value1));
        break;
      case EValueType::INT_64:
        value.vInt64 = *(reinterpret_cast<int64_t*>(value1));
        break;
      case EValueType::UINT_64:
        value.vUInt64 = *(reinterpret_cast<uint64_t*>(value1));
        break;
      case EValueType::FLOAT_32:
        value.vFloat32 = *(reinterpret_cast<float32_t*>(value1));
        break;
      case EValueType::DOUBLE_64:
        value.vDouble64 = *(reinterpret_cast<double64_t*>(value1));
        break;
      case EValueType::VOID:
        value.vVoid8 = cVoidValue;
        break;
      }
    }

    //! \brief Typ danych
    EValueType getType() const noexcept
    {
      return type;
    }

    //! \brief Wartosc
    const UValue getValue() const noexcept
    {
      return value;
    }

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const noexcept
    {
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
  class CSensorData
  {
    const uint32_t idData;           //!< id pomiaru
    const uint8_t idSensor;          //!< id czujnika
    const uint64_t timeStamp;        //!< czas pomiaru
    const ESensorState sensorState;  //!< stan czujnika
    const EDangerLevel dangerLevel;  //!< stopien niebezpieczenstwa
    const CData data; //!< dane pomiaru; dla sensorState != ESensorState.OK pole data przechowuje wartosc cVoidValue

  public:

    CSensorData(const uint32_t idData1,
                const uint8_t& idSensor1,
                const uint64_t timeStamp1,
                const ESensorState& sensorState1,
                const EDangerLevel& dangerLevel1,
                const CData& data1) :
      idData(idData1),
      idSensor(idSensor1),
      timeStamp(timeStamp1),
      sensorState(sensorState1),
      dangerLevel(dangerLevel1),
      data(data1)
    {
    }
    CSensorData(const CSensorData&) = default;
    CSensorData& operator=(const CSensorData&) = default;
    uint32_t getIdData() const noexcept
    {
      return idData;
    }

    uint8_t getIdSensor() const noexcept
    {
      return idSensor;
    }

    uint64_t getTimeStamp() const noexcept
    {
      return timeStamp;
    }

    ESensorState getSensorState() const noexcept
    {
      return sensorState;
    }

    EDangerLevel getDangerLevel() const noexcept
    {
      return dangerLevel;
    }

    const CData getData() const noexcept
    {
      return data;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const noexcept
    {
      return headerSize + data.getSize();
    }
  };

  //! \brief Wiadomosc przesylana protokolem.
  class IMessage{

  public:
    virtual uint32_t getSize() const = 0;
    virtual ~IMessage(){}
  };

  //! \brief Dane z czujnikow wysylane z koncentrator do serwera (SProtocol.type = EMessageType .MONITOR_DATA)
  class CMonitorData : public IMessage
  {
    const uint64_t sendTime;        //!< czas pierwszej proby wyslania pakietu
    const uint8_t sensorsAmount;    //!< ilosc czujnikow
    const std::vector<CSensorData> sensorsData; //!< wektor z danymi z czujnikow

  public:
    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    CMonitorData(const uint64_t& sendTime1,
                 const uint8_t& sensorsAmount1,
                 const std::vector<CSensorData>& sensorsData1) :
      sendTime(sendTime1),
      sensorsAmount(sensorsAmount1),
      sensorsData(sensorsData1)
    {
    }

    CMonitorData(const CMonitorData&) = default;
    CMonitorData& operator=(const CMonitorData&) = default;

    uint64_t getSendTime() const noexcept
    {
      return sendTime;
    }

    uint8_t getSensorAmount() const noexcept
    {
      return sensorsAmount;
    }

    uint32_t getSensorsDataSize() const noexcept
    {
      return static_cast<uint32_t>(sensorsData.size());
    }

    const std::vector<CSensorData>& getSensorsData() const noexcept
    {
      return sensorsData;
    }

    //! \brief Zwraca rozmiar calej struktury
    virtual uint32_t getSize() const noexcept
    {
      uint32_t size = 0;
      std::for_each(sensorsData.begin(), sensorsData.end(),
                    [&size](CSensorData data){size += data.getSize();});
      return headerSize + size;
    }

  };

  //! \brief Id koncentratora
  const int8_t cIdConcentrator = 0xFF;

  //! \brief Pojedyncza wartosc konfigurowalna.
  class CConfigurationValue
  {
    const uint8_t idSensor;        //!< id czujnika lub koncentratora (cIdConcentrator)
    const EConfigurationType configurationType;  //!< opcja konfiguracji
    const CData data;                            //!< wartosc dla danej opcji

  public:

    CConfigurationValue(const uint8_t& idSensor1,
                        const EConfigurationType& configurationType1,
                        const CData& data1) noexcept:
      idSensor(idSensor1),
      configurationType(configurationType1),
      data(data1)
    {

    }
    CConfigurationValue(const CConfigurationValue&) = default;
    CConfigurationValue& operator=(const CConfigurationValue&) = default;
    uint8_t getIdSensor() const noexcept
    {
      return idSensor;
    }

    EConfigurationType getConfigurationType() const noexcept
    {
      return configurationType;
    }

    const CData getData() const noexcept
    {
      return data;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const noexcept
    {
      return headerSize + data.getSize();
    }
  };

  //! \brief Ustawienia konfiguracyjne
  class CConfiguration
  {
    const std::vector<CConfigurationValue> configurations; //!< Tablica opcji konfiguracji

  public:

    CConfiguration(const std::vector<CConfigurationValue> configurations1) noexcept:
      configurations(configurations1)
    {
    }
    CConfiguration(const CConfiguration&) = default;
    CConfiguration& operator=(const CConfiguration&) = default;
    uint8_t getConfigurationsSize() const noexcept
    {
      return static_cast<uint8_t>(configurations.size());
    }

    const std::vector<CConfigurationValue>& getConfigurations() const noexcept
    {
      return configurations;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const noexcept
    {
      uint32_t size = 0;
      std::for_each(configurations.begin(), configurations.end(),
                    [&size](CConfigurationValue data){size += data.getSize();});
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

  //! \brief Potwierdzenie zmiany konfiguracji koncentratora do serwera (SProtocol.type = EMessageType.CONFIGURATION_RESPONSE)
  class CConfigurationResponse : public IMessage
  {
    const EReceiveStatus status;               //!< Status otrzymanego pakietu konfiguracyjnego lub status zmiany konfiguracji
    const uint32_t idRequestPackage;           //!< ID pakietu, ktory zglosil zmiane konfiguracji - pole SProtocol.idPackage
    const CConfiguration currentConfiguration; //!< Konfiguracja koncentratora

  public:

    CConfigurationResponse(const EReceiveStatus& status1,
                           const uint32_t& idRequestPackage1,
                           const CConfiguration& currentConfiguration1) noexcept:
      status(status1),
      idRequestPackage(idRequestPackage1),
      currentConfiguration(currentConfiguration1)
    {}

    CConfigurationResponse(const CConfigurationResponse&) = default;
    CConfigurationResponse& operator=(const CConfigurationResponse&) = default;

    EReceiveStatus getStatus() const noexcept
    {
      return status;
    }

    uint32_t getIdRequestPackage() const noexcept
    {
      return idRequestPackage;
    }

    const CConfiguration getCurrentConfiguration() const noexcept
    {
      return currentConfiguration;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca rozmiar calej struktury
    virtual uint32_t getSize() const noexcept
    {
      return headerSize + currentConfiguration.getSize();
    }
  };

  //! \brief Pojedyncze zadanie opcji konfigurowalnej
  class CRequest
  {
    const uint8_t idSensor;                      //!< id czujnika lub koncentratora (cIdConcentrator)
    const EConfigurationType configurationType; //!< opcja konfiguracji

  public:

    CRequest(const uint8_t idSensor1, const EConfigurationType& configurationType1)  noexcept:
      idSensor(idSensor1), configurationType(configurationType1)
    {
    }

    CRequest(const CRequest&) = default;
    CRequest& operator=(const CRequest&) = default;

    uint8_t getIdSensor() const noexcept
    {
      return idSensor;
    }

    EConfigurationType getConfigurationType() const noexcept
    {
      return configurationType;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;
  };

  //! Prosba koncentratora o wyslanie konfiguracji (SProtocol.type = EMessageType.SERVER_REQUEST)
  class CServerRequest : public IMessage
  {
    const std::vector<CRequest> requests;   //!< tablica o wielkosci requestsSize zadan konfiguracji

  public:

    CServerRequest(const std::vector<CRequest>& requests1) noexcept :
      requests(requests1)
    {
    }

    CServerRequest(const CServerRequest&) = default;
    CServerRequest& operator=(const CServerRequest&) = default;

    uint8_t getRequestsSize() const noexcept
    {
      return static_cast<uint8_t>(requests.size());
    }

    const std::vector<CRequest>& getRequests() const noexcept
    {
      return requests;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca rozmiar calej struktury
    virtual uint32_t getSize() const noexcept
    {
      return headerSize + CRequest::headerSize * requests.size();
    }
  };


  //! \brief Odpowiedz serwera po otrzymaniu danych z czujnikow (SProtocol.type = EMessageType.SERVER_RESPONSE).
  class CServerResponse : public IMessage
  {
    const EReceiveStatus status;        //!< Status otrzymanego pakietu z danymi z czujnikow
    const uint32_t idRequestPackage;    //!< ID pakietu, ktory wyslal dane z czujnikow
    const CConfiguration configuration; //!< Zadanie zmiany konfiguracji koncentratora

  public:

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    CServerResponse(const EReceiveStatus& status1,
                    const uint32_t& idRequestPackage1,
                    const CConfiguration& configuration1) noexcept :
      status(status1),
      idRequestPackage(idRequestPackage1),
      configuration(configuration1)
    {
    }

    CServerResponse(const CServerResponse&) = default;
    CServerResponse& operator=(const CServerResponse&) = default;

    EReceiveStatus getStatus() const noexcept
    {
      return status;
    }

    uint32_t getIdRequestPackage() const noexcept
    {
      return idRequestPackage;
    }

    const CConfiguration getConfiguration() const noexcept
    {
      return configuration;
    }

    //! \brief Zwraca rozmiar calej struktury
    virtual uint32_t getSize() const noexcept
    {
      return headerSize + configuration.getSize();
    }
  };

  //! \brief Struktura protokolu.
  class CProtocol
  {
    //! \brief Wersja protkolu
    const uint8_t version;
    //! \brief Rozmiar laczny danych przesylanych protokolem.
    uint32_t size;
    //! \brief ID koncentratora
    const uint16_t idConcentrator;
    //! \brief ID danych pakietu danych.
    const uint32_t idPackage;
    //! \brief Typ pakietu danych wysylanych protokolem
    const EMessageType type;
    //! \brief Dane wysylane protokolem. Typ danych zalezny do wartoci idPackage.
    const std::shared_ptr<IMessage> message;
    //! \brief Cykliczny kod nadmiarowy dla calej struktury
    uint16_t crc;

  public:

    CProtocol(const uint8_t& version1,
              const uint32_t& size1,
              const uint16_t& idConcentrator1,
              const uint32_t& idPackage1,
              const EMessageType& type1,
              const std::shared_ptr<IMessage>& message1) noexcept :
      version(version1),
      size(size1),
      idConcentrator(idConcentrator1),
      idPackage(idPackage1),
      type(type1),
      message(message1),
      crc(0)
    {
      if (size == 0){
        size = getSize();
      }

    }

    CProtocol(const CProtocol&) = default;
    CProtocol& operator=(const CProtocol&) = default;

    uint8_t getVersion() const noexcept
    {
      return version;
    }


    uint16_t getIdConcentrator() const noexcept
    {
      return idConcentrator;
    }

    uint32_t getIdPackage() const noexcept
    {
      return idPackage;
    }

    EMessageType getType() const noexcept
    {
      return type;
    }

    const std::shared_ptr<IMessage> getMessage() const noexcept
    {
      return message;
    }

    uint16_t getCRC() const noexcept
    {
      return crc;
    }

    void setCRC(uint16_t crc1)
    {
      crc = crc1;
    }

    const static uint32_t headerSize;

    //! \brief Zwraca rozmiar calej struktury
    uint32_t getSize() const
    {
      if (size != 0){
        return size;
      }
      if (!message){
        return headerSize;
      }
      int s1 = message->getSize();
      return headerSize + s1;


    }
  };

}

#endif /* PROTOCOL_H_ */
