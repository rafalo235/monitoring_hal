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

#include "sensors/interfaces/ISensorsManager.h"
#include "sensors/interfaces/SensorsFactory.h"
#include "configuration/interfaces/ConfigurationFactory.h"
#include "communication/interfaces/ConnectionFactory.h"

namespace NEngine{

  using namespace NSensors;

  //! \brief Klasa odpowiedzialna za cykliczne sprawdzanie probek z czujnikow. Co ustalony czas wywolujaca metody odpowiedzialne za
  //! wysylanie danych do serwera.
  class CCycleMonitor
  {
    DSensorManager sensors;
    DConfiguration configuration;
    DConnection connection;

    std::shared_ptr<std::thread> thread;
    std::atomic<bool> threadExit;
    int secondInterval;
    std::time_t checkingSensorsTime;
    std::time_t sendingDataTime;
    std::vector<CSensorData> sensorsData;
    uint32_t idSensorBase;

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


    bool saveSensorDataToFile(const bool warning, const std::vector<CSensorData>& sData);
    bool saveSensorDataToNewFile(const bool warning, const std::vector<CSensorData>& sData);

    void operateReceivedProtocol(const DConnectionResult& result);
  };
}

#endif // CYCLEMONITOR_H
