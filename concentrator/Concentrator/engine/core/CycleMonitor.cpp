#include "CycleMonitor.h"

#include <ctime>
#include <chrono>
#include <vector>
#include <cstdint>

#include "util/Logger.h"

namespace NEngine{
  using namespace NProtocol;


  CCycleMonitor::CCycleMonitor():
    threadExit(false),
    secondInterval(1),
    checkingSensorsTime(0),
    sendingDataTime(0),
    idSensorBase(0)

  {

    sensors = CSensorFactory::getInstance();
    configuration = CConfigurationFactory::getInstance();
    connection = CConnectionFactory::getInstance();
  }

  void CCycleMonitor::runThread()
  {
    thread.reset(new std::thread([&](){run();}));
  }

  // funkcja wywolywana z odrebnego watku
  void CCycleMonitor::run()
  {
    LOG_DEBUG("Engine thread was lunched");
    do{

      // pobiera aktualny czas
      std::time_t curTime = std::chrono::system_clock::to_time_t(
                              std::chrono::system_clock::now());
      // ustala czas obudzenia
      std::time_t wakeUpTime = curTime + secondInterval;

      // usypia watek na okreslony czas
      std::this_thread::sleep_until(
            std::chrono::system_clock::from_time_t(wakeUpTime));

      //LOG_DEBUG("Engine thread woke up.");
      // pobiera aktualny czas
      curTime = std::chrono::system_clock::to_time_t(
                  std::chrono::system_clock::now());

      // sprawdza czy przyszly jakies dane z serwera
      if (connection->isAnyResult())
      {
        DConnectionResult result = connection->getResult();
        switch(result->getStatus())
        {
        case EConnectionStatus::CONNECTION_ERROR:

          break;
        case EConnectionStatus::INPUT_PROTOCOL_FORMAT_ERROR:
          //TODO: blad protokolu wysylanego
          break;
        case EConnectionStatus::OUTPUT_PROTOCOL_FORMAT_ERROR:
          //TODO: blad protokolu otrzymanego
          break;
        case EConnectionStatus::NONE:

          sensorsData.clear();
          operateReceivedProtocol(result);
          break;
        }
      }

      bool warning;
      // czas sprawdzic czujniki
      if (curTime - checkingSensorsTime >= configuration->getCheckingSensorPeriod())
      {
        //LOG_DEBUG("Engine thread checks sensors.");
        warning = checkSensors(false);
       // LOG_DEBUG("Engine thread checked sensors.");
      }
      // czas wyslac dane z czujnikow lub dane byly niepokojace i trzeba je wyslac
      if ((curTime - sendingDataTime >= configuration->getSendingPeriod()) || warning)
      {
       // LOG_DEBUG("Engine thread sends data.");
        if (!warning)
        {
          checkSensors(true);
        }
        if (sensorsData.size() > 0)
        {

            uint8_t amount =
                    static_cast<uint8_t>(configuration->getSensorConfiguration().size());

            CMonitorData* monitorData = new CMonitorData(curTime, amount, sensorsData);
            std::shared_ptr<CMonitorData> monitor(monitorData);
            connection->sendMonitorData(monitor);
        }
      }
      else{
          LOG_DEBUG("Engine thread didn't send data.");
      }
    }while(!threadExit.load(std::memory_order_consume));


  }

  void CCycleMonitor::operateReceivedProtocol(const DConnectionResult& result)
  {
    // TODO: sprawdzic CRC
  }
  bool CCycleMonitor::saveSensorDataToFile(const bool warning, const std::vector<CSensorData>& sData)
  {
    // format pliku:
    // | series1 | ... | seriesN | oldSeries1 | ... | oldSeries2 | oldSeriesSize | SeriesSize | warningSeries | sensors
    // seriesX - dane z pomiarow - kazdy czujnik daje ciagle taki sam typ danych, wiec kazda seria ma taki sam rozmiar
    // oldSeriesX - stare dane z pomiarow - zapisywane sa wszystkie pomiary, lecz dane nic nie wnoszace sa nadpisywywane
    //              oldSeries to pomiary ktore moga byc nadpisane
    // oldSeriesSize - ilosc serii oldSeries
    // seriesSize - ilosc serii, wraz z oldSeries
    // warningSeries - ile serii bylo od ostatniej niebezpiecznej wartosci; -1 oznacza, ze nie bylo takich wartosci
    // sensors - ilosc czujnikow

    // seriesAround - tyle pomiarow przed i po pomiarach niebezpiecznych bedzie zapisane
    const int seriesAround = 10;

    const std::string dir = configuration->getDataPath();
    std::fstream file(dir + "data.dat",
        std::fstream::in | std::fstream::out | std::fstream::binary);

    if (!file.is_open())
    {
      // nie bylo pliku
      return saveSensorDataToNewFile(warning, sData);
    }
    else
    {
      // plik juz byl
      // wczytaj dane na koncu pliku
      int info[4];
      file.seekg(-sizeof(info), std::fstream::end);
      file.read(reinterpret_cast<char*>(info), sizeof(info));
      int oldSeries = info[0];
      int series = info[1];
      int warningSeries = info[2];
      int sensors = info[3];

      if (warning || // niebezpieczne dane
          series < seriesAround || // pierwsze pomiary
          (warningSeries < 2 * seriesAround && // tuz po niebezpiecznych danych
              warningSeries != -1))
      {
        // dopisanie danych
        int oldSeriesSize = oldSeries * sizeof(CSensorData);
        file.seekg(-sizeof(info) - oldSeriesSize, std::fstream::end);
        const int dataSize = sizeof(CSensorData) * sData.size();
        file.write(reinterpret_cast<const char*>(sData.data()), dataSize);

        if (warning)
        {
          // obecny pomiar jest niebezpieczny
          warningSeries = 0;
        }
        else if (warningSeries != -1)
        {
          // byl jakis niebezpieczny wczesniej, wiec zwieksz ilosc pomiarow do niego
          ++warningSeries;
        }
        // jesli byly nieaktualne serie
        if (oldSeries != 0)
        {
          // ile pozostalo pustych offsetow
          if (dataSize <= oldSeriesSize)
          {
            // dane nadpisaly puste offsety
            oldSeries = 0;
          }
          else
          {
            // dane czesciowo nadpisaly puste offsety
            oldSeriesSize = (oldSeriesSize - dataSize);
            file.seekg(-sizeof(info) - oldSeriesSize, std::fstream::end);
            oldSeries = oldSeriesSize / sizeof(int);
          }
        }
        else
        {
          // nie bylo nadpisywania
          ++series;
        }
        int newInfo[] =
        { oldSeries, series, warningSeries, sensors };

        file.write(reinterpret_cast<const char*>(newInfo), sizeof(newInfo));
        file.flush();
        file.close();
        return true;
      }
      else
      {
        // pozycja seriesAround danych ostatnich
        const int appendPosition = sizeof(info)
            + (seriesAround + oldSeries) * sizeof(CSensorData) * sensors;
        file.seekg(-appendPosition, std::fstream::end);
        // jedno wiecej miejsce na nowy wpis
        char buffer[sizeof(CSensorData) * (seriesAround + 1) * sensors];
        // wypelnij buffer danymi bez ostatniej pozycji
        file.read(buffer, sizeof(CSensorData) * seriesAround * sensors);
        // dopisz dane z pomiaru
        memcpy(buffer + sizeof(CSensorData) * seriesAround * sensors,
            reinterpret_cast<const char*>(sData.data()),
            sizeof(CSensorData) * sensors);
        // zapisz dane do pliku
        file.seekg(-appendPosition, std::fstream::end);
        file.write(
            reinterpret_cast<const char*>(buffer + sizeof(CSensorData) * sensors),
            sizeof(CSensorData) * seriesAround * sensors);
        // puste offsety - nastapilo nadpisanie danych, czyli ilosc serii sie nie zmienila
        if (warningSeries != -1)
        {
          ++warningSeries;
        }
        int newInfo[] =
        { oldSeries, series, warningSeries};

        file.write(reinterpret_cast<const char*>(newInfo), sizeof(newInfo));
        file.flush();
        file.close();
        return true;
      }
    }
  }

  bool CCycleMonitor::saveSensorDataToNewFile(const bool warning, const std::vector<CSensorData>& sData)
  {
    const std::string dir = configuration->getDataPath();
    std::ofstream newFile(dir + "data.dat", std::fstream::out | std::fstream::binary);
    if (newFile.is_open())
    {
      newFile.write(reinterpret_cast<const char*>(sData.data()),
                    sizeof(CSensorData) * sData.size());

      const int oldSeries = 0;
      const int allSeries = 1;
      const int warningSeriesOffset = warning ? 0 : -1;
      const int sensors = sData.size();
      int info[] =
      { oldSeries, allSeries, warningSeriesOffset, sensors };

      newFile.write(reinterpret_cast<const char*>(info), sizeof(info));
      newFile.flush();
      newFile.close();
      return true;
    }
    else
    {
      return false;
    }
  }



  void CCycleMonitor::exit(){
    threadExit.store(true, std::memory_order_consume);
    thread->join();
  }

  // Sprawdza czujniki.
  bool CCycleMonitor::checkSensors(bool addToVector){
    bool warningLevel = false;
    const std::vector<DSensorConfiguration> sensorsConf = configuration->getSensorConfiguration();

    std::time_t curTime = std::chrono::system_clock::to_time_t(
                            std::chrono::system_clock::now());

    // jesli funkcja ma nie dodawac do wektora danych z czujnikow, lecz dane sa nie pokojace,
    // zostanie petla powrotrnie uruchomi sprawdzanie czujnikow z zapisem do wektora.
    bool checkOnceAgain;
    do
    {
      checkOnceAgain = false;
      // petla po czujnikach
      for(const DSensorConfiguration& conf : sensorsConf){
        CData data;
        EDangerLevel dangerLvl;
        ESensorState sensorState;
        // jesli czujnik jest wlaczony
        if (conf->isTurnOn())
        {
          // pobierz dane i sprawdz czy czujnik nie jest popsuty
          if (sensors->getSensorData(conf->getSensorId(), data))
          {
            // czujnik dziala poprawnie
            sensorState = ESensorState::OK;
            // czy dane z czujnikow sa powyzej danych alarmowych
            if (conf->getAlarmLvl() < data)
            {
              // wartosc wieksza niz alarmowa
              dangerLvl = EDangerLevel::ALARM;
              warningLevel = true;
              // jesli nie dodawac do wektora, uruchom powtornie przeglad i zapisuj do wektora
              if (!addToVector)
              {
                checkOnceAgain = true;
                break;
              }
            }
            else if (conf->getWarnigLvl() < data)
            {
              // wartosc wieksza niz ostrzegawcza, ale mniejsza niz alarmowa
              dangerLvl = EDangerLevel::WARNING;
              warningLevel = true;
              // jesli nie dodawac do wektora, uruchom powtornie przeglad i zapisuj do wektora
              if (!addToVector)
              {
                checkOnceAgain = true;
                break;
              }
            }
            else
            {
              // wartosc bezpieczna
              dangerLvl = EDangerLevel::NONE;
            }
          }
          else
          {
            // czujnik nie dziala poprawnia
            sensorState = ESensorState::BROKEN;
            dangerLvl = EDangerLevel::ALARM;
          }
        }
        else
        {
          // czujnik nie jest wlaczony
          sensorState = ESensorState::TURN_OFF;
          dangerLvl = EDangerLevel::NONE;
        }
        if (addToVector)
        {

          sensorsData.emplace_back(
                ++idSensorBase,
                            conf->getSensorId(),
                            curTime,
                            sensorState,
                            dangerLvl,
                            data);
          // skoro dane sa juz dodawane do listy, to nie trzeba powtornie przechodzic
          checkOnceAgain = false;
        }
      }
      if (checkOnceAgain)
      {
        // uruchom jeszcze raz sprawdzanie danych, ale tym razem zapamietaj
        addToVector = true;
      }
    }while(checkOnceAgain);

    return warningLevel;
  }




}
