#ifndef CYCLEMONITOR_H
#define CYCLEMONITOR_H

#include <memory>

#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <ctime>
#include <vector>
#include <cstdint>
#include <map>

#include "sensors/interfaces/ISensorsManager.h"
#include "sensors/interfaces/SensorsFactory.h"
#include "engine/core/SensorDataFileManager.h"
#include "configuration/interfaces/ConfigurationFactory.h"
#include "communication/interfaces/ConnectionFactory.h"
#include "communication/interfaces/protocol.h"
#include "util/Time.h"

namespace NEngine{

  using namespace NSensors;
  using namespace NUtil;
  using namespace NProtocol;
  //! \brief Klasa odpowiedzialna za cykliczne sprawdzanie probek z czujnikow. Co ustalony czas wywolujaca metody odpowiedzialne za
  //! wysylanie danych do serwera.
  class CCycleMonitor
  {
    DSensorManager sensors;
    DConfiguration configuration;
    DConnection connection;
    typedef CSensorDataFileManager<CSensorData, 10> DSensorDataFileManager;

    //! \brief smart pointer do obiektu watku
    std::shared_ptr<std::thread> thread;

    //! \brief flaga zatrzymanai watku
    std::atomic<bool> threadExit;

    static const int secondInterval;

    int idSensorDataBase;

    //! \brief czas ostatniego sprawdzenia czujnikow
    STime checkingSensorsTime;
    //! \brief czas ostatniego wyslania danych
    STime sendingDataTime;

    struct SSavedSensorData{
      int idSeries;
      std::vector<CSensorData> sensorDatas;
      STime time;
    };

    SSavedSensorData savedSensorData;

    std::map<uint32_t, SSavedSensorData> sensorSeries;

    struct SOldSensorData
    {
      STime time;
      std::vector<DSensorDataFileManager::SToConfirm> toConfirm;
    };
    std::map<uint32_t, SOldSensorData> oldSensorSeries;

  public:
    CCycleMonitor();

    //! \brief uruchamia watek silnika
    void runThread();

    //! \brief wylacza watek silnika
    void exit();

  private:

    //!
    //! \brief getCurrentConfiguration zwraca aktualna konfiguracje koncentratora w formacie zgodnym do wyslania
    //! \return aktualna konfiguracja koncentratora
    NProtocol::CConfiguration getCurrentConfiguration();

    //!
    //! \brief sendConfigurationChangeResponse wysyla odpowiedz zmiany konfiguracji
    //! \param status status otrzymanego pakietu
    //! \param idRequestPackage id pakietu konfiguracja do zmiany (pakiety wyslanego z serwera)
    void sendConfigurationChangeResponse(const EReceiveStatus status,
                                         const uint32_t idRequestPackage);

    //! \brief funkcja watku silnika
    void run();

    //!
    //! \brief checkSensors Sprawdza dane z czujnikow
    //! \param[in] addToVector Jesli true, to zapisuj dane w CCycleMonitor::sensorsData
    //! \return true jesli pomiary przekroczyly wartosc ostrzegawcza
    bool checkSensors(bool addToVector);

    //!
    //! \brief operateReceivedProtocol obsluguje rezultat wyslanych danych (wywoluje funkcji zapisujace potwierdzenie, odczytuje konfiguracje)
    //! \param result pakiety wysylany, odebrany i status
    void operateReceivedProtocol(const DConnectionResult& result);

    //!
    //! \brief setConfiguration zmienia konfiguracje koncentratora
    //! \param conf nowa konfiguracja
    //! \return true jesli konfiguracja zostala zmieniona
    bool setConfiguration(const NProtocol::CConfiguration& conf);
  };
}

#endif // CYCLEMONITOR_H
