#ifndef SENSORDATEFILEMANAGERTEST_H
#define SENSORDATEFILEMANAGERTEST_H

#ifdef TEST_ENABLE

#include <QDir>
#include <string>
#include <cstdint>
#include "configuration/interfaces/ConfigurationFactory.h"
#include "engine/core/SensorDataFileManager.h"
#include "util/test/Test.h"

namespace NTest
{
  using namespace NEngine;
  using namespace NProtocol;
  using namespace NTest;

  //!
  //! \brief The CSensorDateFileManagerTest class testy jednostkowe CSensorDataFileManager
  class CSensorDateFileManagerTest : public CTest<CSensorDateFileManagerTest>
  {
    //! \brief konfiguracja
    DConfiguration configuration;
    //! \brief typedef CSensorDataFileManager<CSensorData, 10>
    typedef CSensorDataFileManager<CSensorData, 10> DSensorDataFileManager;

    typedef DSensorDataFileManager::SSeriesDataTest DSeriesDataTest;

    //!
    //! \brief removeDir funkcja usuwa katalog i rekurencyjnie wszystko co w nim jest
    //! \param dirName sciezka do katalogu
    //! \return true jesli katalog podany zostal usuniety
    static bool removeDir(const QString& dirName)
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

    //!
    //! \brief recreateDataDirectiory usuwa i tworzy katalog dla danych
    static void recreateDataDirectiory()
    {
      std::string dataPath = CConfigurationFactory::getInstance()->getDataPath();
      QDir qdir(QString(dataPath.c_str()));
      removeDir(qdir.absoluteFilePath("data"));
      if (!qdir.exists("data"))
      {
        qdir.mkdir("data");
      }
    }

    //!
    //! \brief assertData sprawdza poprawnosc zapisanych z podanymi w parametrach
    //! \param buffer dane w .buffer
    //! \param bufferOffset offset w tablicy buffer
    //! \param bufferSize rozmiar rozpatrywany w tablicy buffer
    //! \param warning dane w .warnings
    //! \param warningOffset offset w tablicy warning
    //! \param warningSize rozmiar rozpatrywany w tablicy
    static void assertData(const DSeriesDataTest* buffer,
                    const int bufferOffset,
                    const int bufferSize,
                    const DSeriesDataTest* warning,
                    const int warningOffset,
                    const int warningSize)
    {
      std::vector<DSeriesDataTest> actualBuffer;
      std::vector<DSeriesDataTest> actualWarning;
      DSensorDataFileManager::readFiles(0, actualBuffer, actualWarning);

      assertEq(bufferSize, actualBuffer.size());
      if (buffer != nullptr)
      {
        for(int i = 0; i < bufferSize; ++i)
        {
          const DSeriesDataTest* expect = buffer + bufferOffset + i;
          const DSeriesDataTest actual = actualBuffer[i];
          assertEq(expect->confirmed, actual.confirmed);
          assertEq(expect->seriesId, actual.seriesId);
          assertEq(expect->sensorData.size(), actual.sensorData.size());
        }
      }

      assertEq(warningSize, actualWarning.size());
      if (warning != nullptr)
      {
        for(int i = 0; i < warningSize; ++i)
        {
          const DSeriesDataTest* expect = warning + warningOffset + i;
          const DSeriesDataTest actual = actualWarning[i];
          assertEq(expect->confirmed, actual.confirmed);
          assertEq(expect->seriesId, actual.seriesId);
          assertEq(expect->sensorData.size(), actual.sensorData.size());
        }
      }
    }

  protected:

    //!
    //! \brief setUp przygotowywuje folder dla danych
    virtual void setUp()
    {
      recreateDataDirectiory();
    }

  public:

    //!
    //! \brief CSensorDateFileManagerTest dodaje funkcje testow
    CSensorDateFileManagerTest()
    {
      configuration = CConfigurationFactory::getInstance();
      addTestCase("lessThanSeriesAround_notWarning", &CSensorDateFileManagerTest::lessThanSeriesAround_notWarning);
      addTestCase("moreThanSeriesAround_notWarning", &CSensorDateFileManagerTest::moreThanSeriesAround_notWarning);

    }

    //! \brief lessThanSeriesAround_notWarning sprawdza przypadek, gdy jest mniej serii niz miesci sie w buforze
    void lessThanSeriesAround_notWarning()
    {
      const bool warning = false;
      uint8_t idSensor = 1;
      ESensorState sensorState = ESensorState::OK;
      EDangerLevel dangerLevel = EDangerLevel::NONE;
      int value = 4;
      CData data(EValueType::INT_32, &value);
      const int size = 7;
      DSensorDataFileManager::SSeriesDataTest datas[size];
      for(int i = 0; i < size; ++i)
      {
        uint32_t idData = i;
        uint64_t timeStamp = 10 * i;
        CSensorData sensorData(idData, idSensor, timeStamp, sensorState, dangerLevel, data);
        std::vector<CSensorData> sensorDatas{sensorData};
        int seriesId = DSensorDataFileManager::saveData(warning, sensorDatas);
        datas[i] = DSensorDataFileManager::SSeriesDataTest{seriesId, sensorDatas, false};
      }
      assertData(datas, 0, 7, nullptr, 0, 0);
    }

    //! \brief moreThanSeriesAround_notWarning sprawdza przypadek gdy jest wiecej serii niz miescie sie w buforze
    void moreThanSeriesAround_notWarning()
    {
      const bool warning = false;
      uint8_t idSensor = 1;
      ESensorState sensorState = ESensorState::OK;
      EDangerLevel dangerLevel = EDangerLevel::NONE;
      int value = 4;
      CData data(EValueType::INT_32, &value);
      const int size = 15;
      DSensorDataFileManager::SSeriesDataTest datas[size];
      for(int i = 0; i < size; ++i)
      {
        uint32_t idData = i;
        uint64_t timeStamp = 10 * i;
        CSensorData sensorData(idData, idSensor, timeStamp, sensorState, dangerLevel, data);
        std::vector<CSensorData> sensorDatas{sensorData};
        int seriesId = DSensorDataFileManager::saveData(warning, sensorDatas);
        datas[i] = DSensorDataFileManager::SSeriesDataTest{seriesId, sensorDatas, false};
      }
      assertData(datas, 5, 10, nullptr, 0, 0);
    }

    void lessThanSeriesAround_warning()
    {

    }

  };
}
#endif // TEST_ENABLE
#endif // SENSORDATEFILEMANAGERTEST_H
