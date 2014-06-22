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
    VOID = 10       //!< UValue przechowuje nic nieznaczaca wartosc NProtocol::cVoidValue
  };

  //! \brief Nic nieznaczaca dana wykorzystana dla typu NProtocol::EValueType::VOID
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
    UValue value;     //!< Wartosc o typie NProtocol::EValueType

  public:

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    CData() noexcept {
      value.vVoid8 = cVoidValue;
      type = EValueType::VOID;
    }

    //!
    //! \brief SData Konstruktor nadajacy wartosc
    //! \param[in] type1 Typ wartosci
    //! \param[in] value1 Wskaznik do wartosci
    //!
    CData(const EValueType& type1, void* value1) noexcept
    {
      setValue(type1, value1);
    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CData(const CData& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CData& operator=(const CData& obj) = default;

    bool operator>(const CData& data2) const;
    bool operator<(const CData& data2) const;
    bool operator==(const CData& data2) const;

    //! \brief Zmienia wartosc
    //! \param[in] type1 Typ wartosci
    //! \param[in] value1 Wskaznik do wartosci
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

    //! \brief Zwaraca typ danych
    //! \return Typ danych
    EValueType getType() const noexcept
    {
      return type;
    }

    //! \brief Zwraca wartosc
    //! \return Wartosc
    const UValue getValue() const noexcept
    {
      return value;
    }

    //! \brief Zwraca dokladny rozmiar calej struktury uwzledniajac typ przechowywanych danych.
    //! \return Rozmiar struktury
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
    //! \brief Wartosc czujnika powodujaca poziom niebezpieczenstwa NProtocol::EDangerLevel = NProtocol::EDangerLevel::WARNING
    DANGER_LEVEL = 3,
    //! \brief Wartosc czujnika powodujaca poziom niebezpieczenstwa NProtocol:EDangerLevel = NProtocol::EDangerLevel::ALARM
    ALARM_LEVEL = 4,
    //! \brief Resetowanie koncentratora
    RESET = 5,
    //! \brief zmiana klucza
    KEY_CHANGE = 6,
    //! \brief czas wygasniecia klucza
    KEY_LIFETIME = 7
  };

  //! \brief Typ wiadomosci przekazywanej przez pekiet. Definiuje wartosc IMessage
  enum EMessageType : int8_t
  {
    //! \brief Koncentrator przesyla cykliczne dane z czujnikow. IMessage przechowuje CMonitorData.
    MONITOR_DATA = 0,
    //! \brief Koncentrator potwierdza zmiane konfiguracji. IMessage przechowuje CConfigurationResponse.
    CONFIGURATION_RESPONSE = 1,
    //! \brief Koncentrator prosi o przeslanie konfiguracji. IMessage przechowuje CServerRequest.
    SERVER_REQUEST = 2,
    //! \brief Serwer potwierdza odebrane dane z czujnik z koncentrator
    //!        i ewentualnie przesyla konfiguracje do koncentratora na zadanie koncentratora lub serwera. IMessage przechowuje CServerMonitorResponse.
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
    uint32_t idData;           //!< id pomiaru
    uint8_t idSensor;          //!< id czujnika
    uint64_t timeStamp;        //!< czas pomiaru
    ESensorState sensorState;  //!< stan czujnika
    EDangerLevel dangerLevel;  //!< stopien niebezpieczenstwa
    CData data; //!< dane pomiaru; dla CSensorData::sensorState != ESensorState::OK pole data przechowuje wartosc NProtocol::cVoidValue

  public:

    CSensorData() = default;
    //!
    //! \brief CSensorData Konstruktor
    //! \param[in] idData1 ID pomiaru
    //! \param[in] idSensor1 ID sensoru
    //! \param[in] timeStamp1 Czas
    //! \param[in] sensorState1 Stan czujnika
    //! \param[in] dangerLevel1 Stopien zagozenia
    //! \param[in] data1 Dana pomiaru
    CSensorData(const uint32_t idData1,
                const uint8_t idSensor1,
                const uint64_t timeStamp1,
                const ESensorState sensorState1,
                const EDangerLevel dangerLevel1,
                const CData& data1) :
      idData(idData1),
      idSensor(idSensor1),
      timeStamp(timeStamp1),
      sensorState(sensorState1),
      dangerLevel(dangerLevel1),
      data(data1)
    {
    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CSensorData(const CSensorData& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CSensorData& operator=(const CSensorData& obj) = default;

    //! \brief Zwraca ID pomiaru.
    //! \return ID pomiaru
    uint32_t getIdData() const noexcept
    {
      return idData;
    }

    //! \brief Zwraca ID czujnika.
    //! \return ID czujnika
    uint8_t getIdSensor() const noexcept
    {
      return idSensor;
    }

    //! \brief Zwraca czas
    //! \return Czas
    uint64_t getTimeStamp() const noexcept
    {
      return timeStamp;
    }

    //! \brief Zwraca stan czujnika
    //! \return Stan czujnika
    ESensorState getSensorState() const noexcept
    {
      return sensorState;
    }

    //! \brief Zwraca poziom niebezpieczenstwa.
    //! \return poziom niebezpieczenstwa
    EDangerLevel getDangerLevel() const noexcept
    {
      return dangerLevel;
    }

    //! \brief Zwraca dane pomiaru.
    //! \return dane pomiaru
    const CData getData() const noexcept
    {
      return data;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
    uint32_t getSize() const noexcept
    {
      return headerSize + data.getSize();
    }
  };

  //! \brief Wiadomosc przesylana protokolem. Klasa abstrakcyjna
  class IMessage{

  public:
    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
    virtual uint32_t getSize() const = 0;

    //! \brief Domyslny dekonstruktor
    virtual ~IMessage(){}
  };

  //! \brief Dane z czujnikow wysylane z koncentrator do serwera (CProtocol::type = EMessageType::MONITOR_DATA)
  class CMonitorData : public IMessage
  {
    uint64_t sendTime;        //!< czas pierwszej proby wyslania pakietu
    uint8_t sensorsAmount;    //!< ilosc czujnikow
    std::vector<CSensorData> sensorsData; //!< wektor z danymi z czujnikow

  public:
    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //!
    //! \brief CMonitorData Konstruktor
    //! \param[in] sendTime1 Data wyslania
    //! \param[in] sensorsAmount1 Ilosc czujnikow
    //! \param[in] sensorsData1 Dane pomiarowe
    //!
    CMonitorData(const uint64_t& sendTime1,
                 const uint8_t& sensorsAmount1,
                 const std::vector<CSensorData>& sensorsData1) :
      sendTime(sendTime1),
      sensorsAmount(sensorsAmount1),
      sensorsData(sensorsData1)
    {
    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CMonitorData(const CMonitorData& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CMonitorData& operator=(const CMonitorData& obj) = default;

    //! \brief Zwraca czas wyslania.
    //! \return czas wyslania
    uint64_t getSendTime() const noexcept
    {
      return sendTime;
    }

    //! \brief Zwraca ilosc czujnikow.
    //! \return ilosc czujnikow
    uint8_t getSensorAmount() const noexcept
    {
      return sensorsAmount;
    }

    //! \brief Zwraca ilosc pomiarow.
    //! \return ilosc pomiarow
    uint32_t getSensorsDataSize() const noexcept
    {
      return static_cast<uint32_t>(sensorsData.size());
    }

    //! \brief Zwraca referencje do pomiarow.
    //! \return referencja do pomiarow
    const std::vector<CSensorData>& getSensorsData() const noexcept
    {
      return sensorsData;
    }

    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
    virtual uint32_t getSize() const noexcept
    {
      uint32_t size = 0;
      std::for_each(sensorsData.begin(), sensorsData.end(),
                    [&size](CSensorData data){size += data.getSize();});
      return headerSize + size;
    }

  };

  //! \brief Id koncentratora podawane jako id sensora
  const int8_t cIdConcentrator = 0xFF;

  //! \brief Pojedyncza wartosc konfigurowalna.
  class CConfigurationValue
  {
    uint8_t idSensor;        //!< id czujnika lub koncentratora (NProtocol::cIdConcentrator)
    EConfigurationType configurationType;  //!< opcja konfiguracji
    CData data;                            //!< wartosc dla danej opcji

  public:

    //!
    //! \brief CConfigurationValue Konstruktor
    //! \param[in] idSensor1 Id czujnika
    //! \param[in] configurationType1 Typ konfiguracji
    //! \param[in] data1 Wartosc dla danej konfiguracji
    CConfigurationValue(const uint8_t& idSensor1,
                        const EConfigurationType& configurationType1,
                        const CData& data1) noexcept:
      idSensor(idSensor1),
      configurationType(configurationType1),
      data(data1)
    {
    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CConfigurationValue(const CConfigurationValue& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CConfigurationValue& operator=(const CConfigurationValue& obj) = default;

    //! \brief Zwraca id czujnika.
    //! \return id czujnika
    uint8_t getIdSensor() const noexcept
    {
      return idSensor;
    }

    //! \brief Zwraca typ konfiguracji.
    //! \return typ konfiguracji
    EConfigurationType getConfigurationType() const noexcept
    {
      return configurationType;
    }

    //! \brief Zwraca wartosc dla danej konfiguracji.
    //! \return wartosc dla danej konfiguracji
    const CData getData() const noexcept
    {
      return data;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
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

    //!
    //! \brief CConfiguration Konstruktor
    //! \param[in] configurations1 wektor konfiguracji.
    //!
    CConfiguration(const std::vector<CConfigurationValue> configurations1) noexcept:
      configurations(configurations1)
    {
    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CConfiguration(const CConfiguration& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CConfiguration& operator=(const CConfiguration& obj) = default;

    //! \brief Zwraca wielkosc wektora konfiguracji.
    //! \return wielkosc wektora konfiguracji
    uint8_t getConfigurationsSize() const noexcept
    {
      return static_cast<uint8_t>(configurations.size());
    }

    //! \brief Zwraca referencje do wektora konfiguracji
    //! \return referencje do wektora konfiguracji
    const std::vector<CConfigurationValue>& getConfigurations() const noexcept
    {
      return configurations;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
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
    EReceiveStatus status;               //!< Status otrzymanego pakietu konfiguracyjnego lub status zmiany konfiguracji
    uint32_t idRequestPackage;           //!< ID pakietu, ktory zglosil zmiane konfiguracji - pole SProtocol.idPackage
    CConfiguration currentConfiguration; //!< Konfiguracja koncentratora

  public:

    //!
    //! \brief CConfigurationResponse Konstruktor
    //! \param[in] status1 status paczki, ktorej obiekt jest odpowiedzia
    //! \param[in] idRequestPackage1 id paczki, ktorej obiekt jest odpowiedzia
    //! \param[in] currentConfiguration1 obecna konfiguracja
    CConfigurationResponse(const EReceiveStatus& status1,
                           const uint32_t& idRequestPackage1,
                           const CConfiguration& currentConfiguration1) noexcept:
      status(status1),
      idRequestPackage(idRequestPackage1),
      currentConfiguration(currentConfiguration1)
    {

    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CConfigurationResponse(const CConfigurationResponse& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CConfigurationResponse& operator=(const CConfigurationResponse& obj) = default;

    //! \brief Zwraca status1 status paczki, ktorej obiekt jest odpowiedzia.
    //! \return status1 status paczki, ktorej obiekt jest odpowiedzia
    EReceiveStatus getStatus() const noexcept
    {
      return status;
    }

    //! \brief Zwraca id paczki, ktorej obiekt jest odpowiedzia
    //! \return id paczki, ktorej obiekt jest odpowiedzia
    uint32_t getIdRequestPackage() const noexcept
    {
      return idRequestPackage;
    }

    //! \brief Zwraca obecna konfiguracja
    //! \return obecna konfiguracja
    const CConfiguration& getCurrentConfiguration() const noexcept
    {
      return currentConfiguration;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
    virtual uint32_t getSize() const noexcept
    {
      return headerSize + currentConfiguration.getSize();
    }
  };

  //! \brief Pojedyncze zadanie opcji konfigurowalnej
  class CRequest
  {
    uint8_t idSensor;                      //!< id czujnika lub koncentratora (cIdConcentrator)
    EConfigurationType configurationType; //!< opcja konfiguracji

  public:

    //!
    //! \brief CRequest Konstruktor
    //! \param idSensor1 id czujnika lub koncentrator
    //! \param configurationType1 typ konfiguracji
    CRequest(const uint8_t idSensor1, const EConfigurationType& configurationType1)  noexcept:
      idSensor(idSensor1), configurationType(configurationType1)
    {
    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CRequest(const CRequest& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CRequest& operator=(const CRequest& obj) = default;

    //! \brief Zwraca id czujnika lub koncentrator
    //! \return id czujnika lub koncentrator
    uint8_t getIdSensor() const noexcept
    {
      return idSensor;
    }

    //! \brief Zwraca typ konfiguracji
    //! \return typ konfiguracji
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
    std::vector<CRequest> requests;   //!< tablica o wielkosci requestsSize zadan konfiguracji

  public:

    //! \brief CServerRequest Konstruktor
    //! \param requests1 wektor prosb przeslania konfiguracji
    CServerRequest(const std::vector<CRequest>& requests1) noexcept :
      requests(requests1)
    {
    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CServerRequest(const CServerRequest& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CServerRequest& operator=(const CServerRequest& obj) = default;

    //! \brief Zwraca wielkosc wektora prosb o przeslanie konfiguracji
    //! \return wielkosc wektora prosb o przeslanie konfiguracji
    uint8_t getRequestsSize() const noexcept
    {
      return static_cast<uint8_t>(requests.size());
    }

    //! \brief Zwraca referencje do wektora prosb przeslania konfiguracji
    //! \return referencja do wektora prosb przeslania konfiguracji
    const std::vector<CRequest>& getRequests() const noexcept
    {
      return requests;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
    virtual uint32_t getSize() const noexcept
    {
      return headerSize + CRequest::headerSize * requests.size();
    }
  };


  //! \brief Odpowiedz serwera po otrzymaniu danych z czujnikow (SProtocol.type = EMessageType.SERVER_RESPONSE).
  class CServerResponse : public IMessage
  {
    EReceiveStatus status;        //!< Status otrzymanego pakietu z danymi z czujnikow
    uint32_t idRequestPackage;    //!< ID pakietu, ktory wyslal dane z czujnikow
    CConfiguration configuration; //!< Zadanie zmiany konfiguracji koncentratora

  public:

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //!
    //! \brief CServerResponse Konstruktor
    //! \param status1 status otrzymanej wiadomosci z koncentratora
    //! \param idRequestPackage1 id otrzymanej wiadomosci z koncentratora
    //! \param configuration1 konfiguracja do ustawiania na koncentratorze
    CServerResponse(const EReceiveStatus& status1,
                    const uint32_t& idRequestPackage1,
                    const CConfiguration& configuration1) noexcept :
      status(status1),
      idRequestPackage(idRequestPackage1),
      configuration(configuration1)
    {
    }

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CServerResponse(const CServerResponse& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CServerResponse& operator=(const CServerResponse& obj) = default;

    //! \brief Zwraca status otrzymanej wiadomosci z koncentratora
    //! \return status otrzymanej wiadomosci z koncentratora
    EReceiveStatus getStatus() const noexcept
    {
      return status;
    }

    //! \brief Zwraca id otrzymanej wiadomosci z koncentratora
    //! \return id otrzymanej wiadomosci z koncentratora
    uint32_t getIdRequestPackage() const noexcept
    {
      return idRequestPackage;
    }

    //! \brief Zwraca konfiguracja do ustawiania na koncentratorze
    //! \return konfiguracja do ustawiania na koncentratorze
    const CConfiguration getConfiguration() const noexcept
    {
      return configuration;
    }

    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
    virtual uint32_t getSize() const noexcept
    {
      return headerSize + configuration.getSize();
    }
  };

  //! \brief Struktura protokolu.
  class CProtocol
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
    std::shared_ptr<IMessage> message;
    //! \brief Cykliczny kod nadmiarowy dla calej struktury
    uint16_t crc;

  public:

    //!
    //! \brief CProtocol Konstruktor
    //! \param version1 wersja protokolu
    //! \param size1 dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury;
    //!              jesli 0 to jest obliczany w konstruktorze
    //! \param idConcentrator1 ID koncentratora
    //! \param idPackage1 id paczki
    //! \param type1 typ danych
    //! \param message1 wiadomosc
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

    //! \brief Domyslny konstruktor kopiujacy.
    //! \param[in] obj Obiekt kopiowany
    CProtocol(const CProtocol& obj) = default;

    //! \brief Domyslny operator przypisania.
    //! \param[in] obj Obiekt przypisywany
    CProtocol& operator=(const CProtocol& obj) = default;

    //! \brief Zwraca wersje protokolu
    //! \return wersja protokolu
    uint8_t getVersion() const noexcept
    {
      return version;
    }

    //! \brief Zwraca ID koncentratora
    //! \return ID koncentratora
    uint16_t getIdConcentrator() const noexcept
    {
      return idConcentrator;
    }

    //! \brief Zwraca id pakietu
    //! \return id pakietu
    uint32_t getIdPackage() const noexcept
    {
      return idPackage;
    }

    //! \brief Zwraca typ wiadomosci
    //! \return typ wiadomosci
    EMessageType getType() const noexcept
    {
      return type;
    }

    //! \brief Zwraca wiadomosc
    //! \return wiadomosc
    const std::shared_ptr<IMessage> getMessage() const noexcept
    {
      return message;
    }

    //! \brief Zwraca CRC pakietu
    //! \return CRC pakietu
    uint16_t getCRC() const noexcept
    {
      return crc;
    }

    //! \brief Ustawia CRC
    //! \param[in] crc1
    void setCRC(uint16_t crc1)
    {
      crc = crc1;
    }

    //! \brief Rozmiar samego naglowka
    const static uint32_t headerSize;

    //! \brief Zwraca dokladny, zalezny od danych w NProtocol::CData, rozmiar calej struktury
    //! \return Dokladny rozmiar danych
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
