#define LOGGER_ENABLE
#define LOGGER_FILE_ENABLE
// Ponizszy include definiuje mocka konfiguracji.
// Usuniecie go powoduje uzycie gotowej implementacji konfiguracji
#include "configuration/tests/ConfigurationMock.h"

// Ponizszy include definiuje mocka sensorow.
// Usuniecie go powoduje uzycie gotowej implemtacji sensorow
#include "sensors/tests/CSensorsManagerMock.h"




#include <QCoreApplication>
#include <thread>
#include <chrono>

#include "engine/interfaces/EngineFactory.h"

using namespace NEngine;

int secondInterval = 60;
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  DEngine engine = CEngineFactory::getInstance();
  if(!engine->init())
  {
    LOG_ERROR("Engine inititialization failed.");
  }
  else{
      LOG_DEBUG("Engine was inited.");
      // pobiera aktualny czas
      std::time_t curTime = std::chrono::system_clock::to_time_t(
                              std::chrono::system_clock::now());
      // ustala czas obudzenia
      std::time_t wakeUpTime = curTime + secondInterval;

      // usypia watek na okreslony czas
      std::this_thread::sleep_until(
            std::chrono::system_clock::from_time_t(wakeUpTime));
      engine->exit();
  }
  return a.exec();
}
