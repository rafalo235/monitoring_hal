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
    //! \brief assertData sprawdza podane dane z zapisynami
    //! \param bufferStart1 poczatek danych bufora
    //! \param bufferStop koniec danych bufora
    //! \param warningStart1 poczatek danych niebezpiecznych
    //! \param warningStop koneic danych niebezpiecznych
    template<typename T>
    static void assertData(const T& bufferStart1, const T& bufferStop, const T& warningStart1, const T& warningStop)
    {
      T bufferStart = bufferStart1;
      T warningStart = warningStart1;

      std::vector<DSeriesDataTest> actualBuffer;
      std::vector<DSeriesDataTest> actualWarning;
      DSensorDataFileManager::readFiles(0, actualBuffer, actualWarning);

      const int bufferSize = bufferStop - bufferStart;
      assertEq(bufferSize, actualBuffer.size());

      for(int i = 0; i < bufferSize; ++i)
      {
        const DSeriesDataTest actual = actualBuffer[i];
        assertEq(bufferStart->confirmed, actual.confirmed);
        assertEq(bufferStart->seriesId, actual.seriesId);
        assertEq(bufferStart->sensorData.size(), actual.sensorData.size());
        const CData expectedData = bufferStart->sensorData.begin()->getData();
        const CData actualData = actual.sensorData.begin()->getData();
        assertEq(static_cast<int>(expectedData.getType()), static_cast<int>(actualData.getType()));
        assertEq(expectedData.getValue().vUInt64, actualData.getValue().vUInt64);

        ++bufferStart;
      }

      const int warningSize = warningStop - warningStart;
      assertEq(warningSize, actualWarning.size());

      for(int i = 0; i < warningSize; ++i)
      {
        const DSeriesDataTest actual = actualWarning[i];
        assertEq(warningStart->confirmed, actual.confirmed);
        assertEq(warningStart->seriesId, actual.seriesId);
        assertEq(warningStart->sensorData.size(), actual.sensorData.size());
        ++warningStart;
      }

    }

    //!
    //! \brief simulate simuluje
    //! \param size ilosc serii
    //! \param warnings wektor pozycji serii niebezpiecznych
    void simulate(const int size, std::vector<int>& warnings)
    {
      std::sort(warnings.end(), warnings.end());

      std::vector<DSeriesDataTest> buffersAll;
      std::vector<DSeriesDataTest> warningsAll;

      std::vector<int>::const_iterator it = warnings.begin();
      std::vector<int>::const_iterator end = warnings.end();
      int seriesAfterLastWarning = -1;

      for (int i = 0; i < size; ++i)
      {
        bool warning = false;

        for(; it != end && i > *it; ++it);
        if(it != end  && *it == i)
        {
          warning = true;
          if (seriesAfterLastWarning == -1 || seriesAfterLastWarning > 10)
          {
            int copySize = 0;
            if (seriesAfterLastWarning == -1)
            {
              copySize = std::min(static_cast<int>(buffersAll.size()), 10);
            }
            else
            {
              copySize = std::min(seriesAfterLastWarning, 10);
            }

            const std::vector<DSeriesDataTest>::iterator end = buffersAll.end();
            warningsAll.insert(warningsAll.end(), end - copySize, end);
            buffersAll.erase(end - copySize, end);
          }
          seriesAfterLastWarning = 0;
        }

        DSensorDataFileManager::SSeriesDataTest data = createData(warning);
        if (seriesAfterLastWarning > 10 || seriesAfterLastWarning == -1)
        {
          buffersAll.push_back(data);
        }
        else
        {
          warningsAll.push_back(data);
        }
        if (seriesAfterLastWarning != -1)
        {
          ++seriesAfterLastWarning;
        }
      }

      int bufferSize;
      if (seriesAfterLastWarning == -1)
      {
        bufferSize = buffersAll.size();
        bufferSize = bufferSize > 10 ? 10 : bufferSize;
      }
      else
      {
        bufferSize = (seriesAfterLastWarning - 11);
        bufferSize = bufferSize < 0 ? 0 : bufferSize;
      }

      assertData(buffersAll.end() - bufferSize, buffersAll.end(), warningsAll.begin(), warningsAll.end());
    }

    //!
    //! \brief createData tworzy dane SSeriesDataTest i wywoluje DSensorDataFileManager::saveData
    //! \param warning true dla tworzenie danych niebezpiecznych
    //! \return zwraca SSeriesDataTest, ktory zawiera dane o serii
    DSensorDataFileManager::SSeriesDataTest createData(const bool warning)
    {
      static uint32_t idData = 0;
      const uint8_t idSensor = 1;
      const ESensorState sensorState = ESensorState::OK;
      const EDangerLevel dangerLevel = EDangerLevel::NONE;
      const uint64_t timeStamp = idSensor;
      const long long value = 4;
      const CData data(EValueType::UINT_64, &value);
      CSensorData sensorData(++idData, idSensor, timeStamp, sensorState, dangerLevel, data);
      std::vector<CSensorData> sensorDatas{sensorData};
      int seriesId = DSensorDataFileManager::saveData(warning, sensorDatas);

      return DSensorDataFileManager::SSeriesDataTest{seriesId, sensorDatas, false};
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
      addTestCase("lessThanSeriesAround_warning", &CSensorDateFileManagerTest::lessThanSeriesAround_warning);
      addTestCase("moreThanSeriesAround_warning", &CSensorDateFileManagerTest::moreThanSeriesAround_warning);
      addTestCase("lessThanSeriesAround_5warning", &CSensorDateFileManagerTest::lessThanSeriesAround_5warning);
      addTestCase("moreThanSeriesAround_8warning", &CSensorDateFileManagerTest::moreThanSeriesAround_8warning);
      addTestCase("moreThanSeriesAround_8_33warning", &CSensorDateFileManagerTest::moreThanSeriesAround_8_33warning);
      addTestCase("moreThanSeriesAround_12_43warning", &CSensorDateFileManagerTest::moreThanSeriesAround_12_43warning);


    }

    //! \brief lessThanSeriesAround_notWarning sprawdza przypadek, gdy jest mniej serii niz miesci sie w buforze
    void lessThanSeriesAround_notWarning()
    {
      std::vector<int> warnings;
      const int size = 3;
      simulate(size, warnings);
    }

    //! \brief moreThanSeriesAround_notWarning sprawdza przypadek gdy jest wiecej serii niz miescie sie w buforze
    void moreThanSeriesAround_notWarning()
    {
      std::vector<int> warnings;
      const int size = 15;
      simulate(size, warnings);
    }

    //! \brief lessThanSeriesAround_warning sprawdza przypadek, gdy jest mniej serii niz miescie w buforze i sa to serie niebezpieczne
    void lessThanSeriesAround_warning()
    {
      std::vector<int> warnings;
      const int size = 3;
      for(int i = 0; i < size; ++i)
      {
        warnings.push_back(i);
      }
      simulate(size, warnings);
    }

    //! \brief moreThanSeriesAround_warning sprawdza przypadek gdy jest wiecej serii niz miescie sie w buforze i sa to serie niebezpieczne
    void moreThanSeriesAround_warning()
    {
      std::vector<int> warnings;
      const int size = 15;
      for(int i = 0; i < size; ++i)
      {
        warnings.push_back(i);
      }
      simulate(size, warnings);
    }

    //! \brief lessThanSeriesAround_5warning sprawdza przypadek, gdy 5 pierwszych serii jest niebezpiecznych, a razem jest 8 serii
    void lessThanSeriesAround_5warning()
    {
      std::vector<int> warnings;
      const int size = 8;
      for(int i = 5; i < size; ++i)
      {
        warnings.push_back(i);
      }
      simulate(size, warnings);
    }

    //! \brief moreThanSeriesAround_8warning sprawdza przypadek 20 serii, 8 seria jest niebezpieczna
    void moreThanSeriesAround_8warning()
    {
      std::vector<int> warnings;
      const int size = 20;
      warnings.push_back(8);
      simulate(size, warnings);
    }

    //! \brief moreThanSeriesAround_8_33warning sprawdza przypadek 40 serii, 8 i 33 seria sa niebezpieczna
    void moreThanSeriesAround_8_33warning()
    {
      std::vector<int> warnings;
      const int size = 40;
      warnings.push_back(8);
      warnings.push_back(33);

      simulate(size, warnings);
    }

    //! \brief moreThanSeriesAround_12_43warning sprawdza przypadek 50 serii, 12, 33 seria sa niebezpieczna
    void moreThanSeriesAround_12_43warning()
    {
      std::vector<int> warnings;
      const int size = 50;
      warnings.push_back(12);
      warnings.push_back(33);

      simulate(size, warnings);
    }
  };
}
#endif // TEST_ENABLE
#endif // SENSORDATEFILEMANAGERTEST_H
