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

    void runThread();
    void exit();
  private:
    void run();

    bool checkSensors(bool addToVector);
    void saveSensorsData();

    void operateReceivedProtocol(const DConnectionResult& result);
  };
}

#endif // CYCLEMONITOR_H
