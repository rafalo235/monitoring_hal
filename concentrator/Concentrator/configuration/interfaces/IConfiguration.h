#ifndef ICONFIGURATION_H
#define ICONFIGURATION_H

#include <memory>
#include <string>

#include "communication/interfaces/protocol.h"
#include "configuration/interfaces/ISensorConfiguration.h"

namespace NEngine{

  //! \brief Smart pointer do konfiguracji konkrentego czujnika
  typedef std::shared_ptr<ISensorConfiguration> DSensorConfiguration;

  //!
  //! \brief The IConfiguration class Klasa abstrakcyjna konfiguracji
  class IConfiguration
  {
  public:
    //! \brief readConfiguration Zmienia konfiguracje i zapisuje ja do pliku
    //! \return true jesli wszystko ok, false jesli nie
    virtual bool readConfiguration() = 0;

    //! \brief getCheckingSensorPeriod Zwraca przerwe pomiedzy sprawdzaniem danych z czujnikow
    //! \return przerwa pomiedzy sprawdzaniem danych w sekundach
    virtual uint16_t getCheckingSensorPeriod() const = 0;

    //! \brief getIdConcentrator Zwraca id koncentratora odczytane z pliku konfiguracyjnego
    //! \return id koncentratora
    virtual uint16_t getIdConcentrator() const = 0;

    //! \brief getSendingPeriod Zwraca przerwe pomiedzy wysylanymi danymi
    //! \return przerwa pomiedzy wysylanymi danymi w sekundach
    virtual uint16_t getSendingPeriod() const = 0;

    //! \brief getSensorConfiguration Zwraca konfiguracje czujnikow
    //! \return konfiguracja czujnikow
    virtual const std::vector<DSensorConfiguration>& getSensorConfiguration()const =0;

    //! \brief getSaveSDCardIfOnlineEnable Zwraca konfiguracje zapisu na karte SD
    //! \return true, jesli dane maja byc ciagle zapisywane na karte SD nawet jeśli polaczenie z serwerem jest OK
    virtual bool getSaveSDCardIfOnlineEnable() const = 0;

    //! \brief getServerUrl Zwraca URL serwera
    //! \return  URL serwera
    virtual const std::string getServerUrl() const = 0;

    //! \brief setCheckingSensorPeriod Zmienia czas pomiedzy sprawdzaniem czujnikow
    //! \param checkingSensorPeriod1 czas w sekundach
    //! \return true jesli zapis sie udal; false jesli nie
    virtual bool setCheckingSensorPeriod(const uint16_t checkingSensorPeriod1) = 0;

    //! \brief setSendingPeriod Zmienia czas pomiedzy wysylaniem danych
    //! \param sendingPeriod1 czas w sekundach
    //! \return true jesli zapis sie udal; false jesli nie
    virtual bool setSendingPeriod(const uint16_t sendingPeriod1) = 0;

    //! \brief setWarningLevel Zmienia granice danych ostrzegawczych
    //! \param idSensor id czujnika
    //! \param value wartosc
    //! \return true jesli zapis sie udal; false jesli nie
    virtual bool setWarningLevel(const int8_t idSensor,
                                 const CData& value) = 0;

    //! \brief setAlarmLevel Zmienia granice danych alarmowych
    //! \param idSensor id czujnika
    //! \param value wartosc
    //! \return true jesli zapis sie udal; false jesli nie
    virtual bool setAlarmLevel(const int8_t idSensor,
                               const CData& value) = 0;

    //! \brief setTurnOn Wylacza lub wlacza czujnik
    //! \param idSensor id czujnika
    //! \param turnOn true wlacza, false wylacza
    //! \return true jesli zapis sie udal; false jesli nie
    virtual bool setTurnOn(const int8_t idSensor,
                           const bool turnOn) = 0;

    //! \brief setSaveSDCardIfOnlineEnable Ustawia zapis na karte SD nawet jak jest polaczenie z serwerem
    //! \param enable true walcza zapis na karte SD, false wylacza
    //! \return true jesli zapis sie udal; false jesli nie
    virtual bool setSaveSDCardIfOnlineEnable(bool enable) = 0;

    //! \brief Domyslny dekonstruktor
    virtual ~IConfiguration() = default;
  };

}
#endif // ICONFIGURATION_H
