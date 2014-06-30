#include "CycleMonitor.h"

#include <vector>
#include <cstdint>

#include "util/Logger.h"
#include "util/Time.h"
#include "engine/core/SensorDataFileManager.h"

namespace NEngine{
  using namespace NProtocol;
  using namespace NUtil;

  const int CCycleMonitor::secondInterval = 1;


  CCycleMonitor::CCycleMonitor():
    threadExit(false)
  {
    sensors = CSensorFactory::getInstance();
    configuration = CConfigurationFactory::getInstance();
    connection = CConnectionFactory::getInstance();
    DSensorDataFileManager::init();
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

      // usypia watek na okreslony czas
      std::this_thread::sleep_for(std::chrono::seconds(secondInterval));

      // pobiera aktualny czas
      STime curTime = CTime::now();

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
          operateReceivedProtocol(result);
          break;
        }
      }

      bool warning;
      // czas sprawdzic czujniki
      if (curTime - checkingSensorsTime >= configuration->getCheckingSensorPeriod())
      {
        warning = checkSensors(false);
        checkingSensorsTime = curTime;
      }
      // czas wyslac dane z czujnikow lub dane byly niepokojace i trzeba je wyslac
      if ((curTime - sendingDataTime >= configuration->getSendingPeriod()) || warning)
      {
       // LOG_DEBUG("Engine thread sends data.");
        if (!warning)
        {
          checkSensors(true);
        }
        if (savedSensorData.sensorDatas.size() > 0)
        {

            uint8_t amount =
                    static_cast<uint8_t>(configuration->getSensorConfiguration().size());

            CMonitorData* monitorData = new CMonitorData(
                                          curTime.getTime(),
                                          amount,
                                          savedSensorData.sensorDatas);
            std::shared_ptr<CMonitorData> monitor(monitorData);
            uint32_t sendIdPackage = connection->sendMonitorData(monitor);
            sensorSeries[sendIdPackage] = savedSensorData;
        }
      }
      else{
          LOG_DEBUG("Engine thread didn't send data.");
      }
    }while(!threadExit.load(std::memory_order_consume));


  }

  void CCycleMonitor::operateReceivedProtocol(const DConnectionResult& result)
  {
    //TODO: sprawdzic CRC
    std::shared_ptr<CProtocol> receivedProtocol = result->getReceivedProtocol();
    if (receivedProtocol->getIdConcentrator() != configuration->getIdConcentrator())
    {
      LOG_ERROR("Wrong id of concentrator in received package");
      //TODO
    }
    else
    {
      std::shared_ptr<CServerResponse> responseMessage =
          std::dynamic_pointer_cast<CServerResponse>(receivedProtocol->getMessage());

      // potwierdz wyslany pakiet
      const uint32_t idRequestPackage = responseMessage->getIdRequestPackage();
      std::map<uint32_t, SSavedSensorData>::iterator it = sensorSeries.find(idRequestPackage);
      if (it != sensorSeries.end())
      {
        STime dif = CTime::now() - it->second.time;
        std::vector<int> idSeries{it->second.idSeries};
        DSensorDataFileManager::confirm(dif.getDay(), idSeries);
        sensorSeries.erase(it);
        //LOG_OUTPUT(DSensorDataFileManager::coutFiles(0));
      }
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
    savedSensorData.sensorDatas.clear();;

    STime curTime = CTime::now();

    // dane do zapisu do pliku
    // jesli funkcja ma nie dodawac do wektora danych z czujnikow, lecz jesli dane sa niepokojace,
    // petla zostanie powrotrnie uruchomiona ze sprawdzaniem czujnikow i zapisem do wektora.
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
          if (sensors->getSensorData(conf, data))
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

          savedSensorData.sensorDatas.emplace_back(
                ++idSensorDataBase,
                            conf->getSensorId(),
                            curTime.getTime(),
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

    // jesli sa jakies dane, to je zapisz
    if (savedSensorData.sensorDatas.size() > 0)
    {
      savedSensorData.idSeries = DSensorDataFileManager::saveData(warningLevel, savedSensorData.sensorDatas);
      savedSensorData.time = CTime::now();
      //LOG_OUTPUT(DSensorDataFileManager::coutFiles(0));
    }
    return warningLevel;
  }




}
