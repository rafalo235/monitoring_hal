#define LOGGER_ENABLE
#define LOGGER_FILE_ENABLE

#include <QCoreApplication>

#ifdef TEST_ENABLE
#include "util/test/Test.h"

#else //TEST_ENABLE

#include <thread>
#include <chrono>

#include "engine/interfaces/EngineFactory.h"
#include "engine/test/SensorDateFileManagerTest.h"
#include "util/FileHelper.h"

int programLifeTime = 1300;
using namespace NEngine;

#endif // TEST_ENABLE


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

#ifdef TEST_ENABLE
  const std::string startTestTxt("TEST START");
  NTest::CTestBase::print('=', &startTestTxt);
  NTest::CTestBase::runAllTests();
  const std::string stopTestTxt("TEST STOP");
  NTest::CTestBase::print('=', &stopTestTxt);

#else //TEST_ENABLE

  // usunmy stare pliki z danymi
  NUtil::CFileHelper::recreateDataDirectiory();
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
      std::time_t wakeUpTime = curTime + programLifeTime;

      // usypia watek na okreslony czas
      std::this_thread::sleep_until(
            std::chrono::system_clock::from_time_t(wakeUpTime));
      engine->exit();
  }
#endif //TEST_ENABLE
  return a.exec();
}
