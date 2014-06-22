#ifndef SENSORDATEFILEMANAGERTEST_H
#define SENSORDATEFILEMANAGERTEST_H

#define TEST
#ifdef TEST
#include <QDir>
#include <string>
#include "configuration/interfaces/ConfigurationFactory.h"
#include "engine/core/SensorDataFileManager.h"
#include "util/test/Test.h"

namespace NTest
{
  using namespace NEngine;
  using namespace NProtocol;
  using namespace NTest;
  class CSensorDateFileManagerTest : public CTest
  {
    DConfiguration configuration;
    typedef CSensorDataFileManager<CSensorData, 10> DSensorDataFileManager;
    typedef DSensorDataFileManager::SSeriesDataTest DSeriesDataTest;

    bool removeDir(const QString& dirName)
    {
      bool result = true;
      QDir dir(dirName);

      if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
          if (info.isDir()) {
            result = removeDir(info.absoluteFilePath());
          }
          else {
            result = QFile::remove(info.absoluteFilePath());
          }

          if (!result) {
            return result;
          }
        }
        result = dir.rmdir(dirName);
      }
      return result;
    }

    void recreateDataDirectiory()
    {
      std::string dataPath = CConfigurationFactory::getInstance()->getDataPath();
      QDir qdir(QString(dataPath.c_str()));
      removeDir(qdir.absoluteFilePath("data"));
      if (!qdir.exists("data"))
      {
        qdir.rmdir("data");
      }
    }



    bool assertData(const DSeriesDataTest* buffer,
                    const int bufferOffset,
                    const int bufferSize,
                    const DSeriesDataTest* warning,
                    const int warningOffset,
                    const int warningSize)
    {
      DSeriesDataTest actualBuffer;
      DSeriesDataTest actualWarning;
      DSensorDataFileManager::readFiles(0, actualBuffer, actualWarning);

      if (buffer != nullptr)
      {
        for(int i = 0; i < bufferSize; ++i)
        {
          DSeriesDataTest* expect = buffer + bufferOffset + i;
          DSeriesDataTest actual = actualBuffer[i];
          assert(expect->confirmed, actual.confirmed);
          assert(expect->seriesId, actual.seriesId);
          assert(expect->sensorData.size(), actual.sensorData.size());
        }
      }
      if (buffer != nullptr)
      {
        for(int i = 0; i < warningSize; ++i)
        {
          DSeriesDataTest* expect = warning + warningOffset + i;
          DSeriesDataTest actual = actualWarning[i];
          assert(expect->confirmed, actual.confirmed);
          assert(expect->seriesId, actual.seriesId);
          assert(expect->sensorData.size(), actual.sensorData.size());
        }
      }
    }
  protected:
    void setUp()
    {
      recreateDataDirectiory();
    }
  public:

    CSensorDateFileManagerTest()
    {
      configuration = CConfigurationFactory::getInstance();
    }


    void lessThanSeriesAround_notWarning()
    {
      const bool warning = false;
      uint8_t idSensor = 1;
      ESensorState sensorState = ESensorState::OK;
      EDangerLevel dangerLevel = EDangerLevel::NONE;
      int value = 4;
      CData data(EValueType::INT_32, &value);
      const int size = 15;
      DSensorDataFileManager::SSeriesDataTest datas[size];
      for(int i = 0; i < 15; ++i)
      {
        uint32_t idData = i;
        uint64_t timeStamp = 10 * i;
        CSensorData sensorData(idData, idSensor, timeStamp, sensorState, dangerLevel, data);
        int seriesId = DSensorDataFileManager::saveData(warning, sensorData);
        datas[i] = DSensorDataFileManager::SSeriesDataTest{seriesId, sensorData, false};
      }


      assertData(datas, 5, 10, nullptr, 0, 0);



    }

  };
}
#endif
#endif // SENSORDATEFILEMANAGERTEST_H
