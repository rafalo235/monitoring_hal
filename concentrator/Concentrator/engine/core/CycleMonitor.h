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

  public:
    CCycleMonitor();

    //! \brief uruchamia watek silnika
    void runThread();

    //! \brief wylacza watek silnika
    void exit();

  private:

    //! \brief funkcja watku silnika
    void run();

    //!
    //! \brief checkSensors Sprawdza dane z czujnikow
    //! \param[in] addToVector Jesli true, to zapisuj dane w CCycleMonitor::sensorsData
    //! \return true jesli pomiary przekroczyly wartosc ostrzegawcza
    bool checkSensors(bool addToVector);

    void operateReceivedProtocol(const DConnectionResult& result);
  };
}

#endif // CYCLEMONITOR_H
