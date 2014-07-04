#ifndef CCONFIGURATION_H
#define CCONFIGURATION_H

#include <fstream>
#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>
#include <mutex>

#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/IConfiguration.h"
#include "configuration/core/SensorConfiguration.h"

namespace NEngine {

  using namespace NProtocol;
  class CConfigurationFactory;

  //!
  //! \brief The CConfiguration class Klasa konfiguracji
  //! \author Marcin Serwach
  class CConfiguration : public IConfiguration
  {
    friend class CConfigurationFactory;

  protected:

    //! \brief ID koncentratora do wysylania
    uint16_t idConcentrator;

    //! \brief Przerwa pomiedzy wysylanymi danymi
    uint16_t sendingPeriod;

    //! \brief Przerwa pomiedzy sprawdzaniem danych z czujnikow
    uint16_t checkingSensorPeriod;

    //! \brief Konfiguracja dla poszczegolnych sensorow
    std::vector<DSensorConfiguration> sensors;

    //! \brief czy zapisywac dane na SD nawet jesli jest polaczenie z serwerem
    bool saveSDCardIfOnlineEnable;

    //! \brief adres serwera
    std::string serverUrl;

    //! \brief sciezka zapisu danych na karcie SD
    std::string dataPath;

    //! \brief sciezka pliku z konfiguracja
    static const std::string fileName;

    //! \brief konfiguracja jest dostepna z wielu watkow, wiec potrzebny jest mutex
    mutable std::mutex mutex;

    //! \brief Wczytuje konfiguracje z pliku
    //! \return true jesli wszystko ok, false jesli nie
    bool saveConfiguration() const;
    //!
    //! \brief Zmienia ustawienia. Wywoluje funkcje zapisu do pliku.
    //! \param[in,out] currentValue obecna wartosc
    //! \param[in] newValue nowa wartosc
    template<typename T>
    bool changeValue(T& currentValue, const T& newValue);

    //!
    //! \brief setSensorValue Zmienia wartosci konfiguracyjne pojedynczego czujnika (wartosci typu zlozonego)
    //! \param idSensor id czujnika
    //! \param getter wskaznik do gettera wartosci zmienianej
    //! \param setter wskaznik do setter wartosci zmienianej
    //! \param newValue nowa wartosc
    //! \return true jesli udalo sie zmienic wartosc; false jesli nie
    template<typename T>
    bool setSensorValue(const int8_t idSensor, const T (CSensorConfiguration::*getter)() const,
                        void (CSensorConfiguration::*setter)(const T&), const T& newValue);

    //!
    //! \brief setSensorValue Zmienia wartosci konfiguracyjne pojedynczego czujnika (wartosci typu prymitywnego)
    //! \param idSensor id czujnika
    //! \param getter wskaznik do gettera wartosci zmienianej
    //! \param setter wskaznik do setter wartosci zmienianej
    //! \param newValue nowa wartosc
    //! \return true jesli udalo sie zmienic wartosc; false jesli nie
    template<typename T>
    bool setSensorValue(const int8_t idSensor, T (CSensorConfiguration::*getter)() const,
                        void (CSensorConfiguration::*setter)(const T), const T& newValue);

    //!
    //! \brief getValue wrapper do getterow, zapewniajacy, ze dane beda odczytane poprawnie (nie w trakcie zmieniania)
    //! \param value wartosc
    //! \return wartosc
    template<typename T>
    inline T getValue(const T& value) const
    {
      // funkcje zmieniajce wartosci lock'uja tego mutexa
      mutex.lock();
      mutex.unlock();
      return value;
    }

    //!
    //! \brief saveString zapisuje string do pliku wraz z informacja o dlugosci
    //! \param file plik
    //! \param stringToSave string do zapisu
    static inline void writeString(std::ofstream& file,
        const std::string& stringToSave)
    {
      const uint8_t length = static_cast<uint8_t>(stringToSave.size());

      file.write(reinterpret_cast<const char*>(&length),
          sizeof(length));

      file.write(reinterpret_cast<const char*>(stringToSave.c_str()),
          length);
    }

    //!
    //! \brief readString Odczytuje string
    //! \param file plik
    //! \return odczytany string
    static inline std::string readString(std::ifstream& file)
    {
      uint8_t length;
      file.read(reinterpret_cast<char*>(&length), sizeof(length));
      std::string stringBuf(length, '\0');

      file.read(&stringBuf[0], length);
      return stringBuf;
    }

  public:

    CConfiguration() = default;

    CConfiguration(const CConfiguration&) = delete;
    CConfiguration& operator=(const CConfiguration&) = delete;

    //! \brief Zmienia konfiguracje i zapisuje ja do pliku
    //! \return true jesli wszystko ok, false jesli nie
    virtual bool readConfiguration();

    //! IConfiguration::getCheckingSensorPeriod
    virtual uint16_t getCheckingSensorPeriod() const
    {
      return getValue(checkingSensorPeriod);
    }

    //! IConfiguration::getIdConcentrator()
    virtual uint16_t getIdConcentrator() const
    {
      return getValue(idConcentrator);
    }

    //! IConfiguration::getSendingPeriod()
    virtual uint16_t getSendingPeriod() const
    {
      return getValue(sendingPeriod);
    }

    //! IConfiguration::getSensorConfiguration()
    virtual const std::vector<DSensorConfiguration> getSensorConfiguration() const
    {
      return getValue(sensors);
    }

    //! IConfiguration::getServerUrl()
    virtual const std::string getServerUrl() const
    {
      return getValue(serverUrl);
    }

    //! IConfiguration::setCheckingSensorPeriod(const uint16_t)
    virtual bool setCheckingSensorPeriod(const uint16_t checkingSensorPeriod1)
    {
      return changeValue(checkingSensorPeriod, checkingSensorPeriod1);
    }

    //! IConfiguration::setSendingPeriod(const uint16_t)
    virtual bool setSendingPeriod(const uint16_t sendingPeriod1)
    {
      return changeValue(sendingPeriod, sendingPeriod1);
    }


    //! IConfiguration::setWarningLevel(const int8_t, const CData&)
    virtual bool setWarningLevel(const int8_t idSensor,
                                 const CData& value)
    {
      return setSensorValue(idSensor, &CSensorConfiguration::getWarnigLvl,
                            &CSensorConfiguration::setWarningLvl, value);
    }

    //! IConfiguration::setAlarmLevel(const int8_t, const CData&)
    virtual bool setAlarmLevel(const int8_t idSensor,
                               const CData& value)
    {

      return setSensorValue(idSensor, &CSensorConfiguration::getAlarmLvl,
                            &CSensorConfiguration::setAlarmLvl, value);
    }

    //! IConfiguration::setTurnOn(const int8_t, const bool)
    virtual bool setTurnOn(const int8_t idSensor,
                           const bool turnOn)
    {
      bool (CSensorConfiguration::*getter)() const = &CSensorConfiguration::isTurnOn;
      void (CSensorConfiguration::*setter)(const bool) =&CSensorConfiguration::setTurnOn;
      return setSensorValue(idSensor, getter,
                            setter, turnOn);
    }

    //! IConfiguration::getSaveSDCardIfOnlineEnable()
    virtual bool getSaveSDCardIfOnlineEnable() const
    {
      return getValue(saveSDCardIfOnlineEnable);
    }

    //! IConfiguration::setSaveSDCardIfOnlineEnable(bool)
    virtual bool setSaveSDCardIfOnlineEnable(bool enable)
    {
      return changeValue(saveSDCardIfOnlineEnable, enable);
    }

    //! IConfiguration::getDataPath()
    virtual const std::string getDataPath() const
    {
      return getValue(dataPath);
    }
  };
}
#endif // CCONFIGURATION_H
