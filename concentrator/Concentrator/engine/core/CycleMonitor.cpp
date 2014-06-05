#include "CycleMonitor.h"

#include <ctime>
#include <chrono>
#include <vector>
#include <cstdint>

#include "communication/interfaces/protocolUtil.h"
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

  void CCycleMonitor::runThread(){
    thread.reset(new std::thread([&](){run();}));
  }

  // funkcja wywolywana z odrebnego watku
  void CCycleMonitor::run(){
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
          saveSensorsData();
          break;
        case EConnectionStatus::INPUT_PROTOCOL_FORMAT_ERROR:
          //TODO: blad protokolu wysylanego
          break;
        case EConnectionStatus::OUTPUT_PROTOCOL_FORMAT_ERROR:
          //TODO: blad protokolu otrzymanego
          break;
        case EConnectionStatus::NONE:
          if (configuration->getSaveSDCardIfOnlineEnable())
          {
            // zapisywanie danych nawet jesli byly
            saveSensorsData();
          }
         // sensorsData.clear();
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
        if (!warning){
          checkSensors(true);
        }
        if (sensorsData.size() > 0){

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

  void CCycleMonitor::saveSensorsData()
  {
    //TODO: zapis na karte SD
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

    bool checkOnceAgain;
    do{
      checkOnceAgain = false;

      for(const DSensorConfiguration& conf : sensorsConf){

        if (conf->isTurnOn())
        {
          //TODO: jak sprawdzic czy czujnik nie jest rozwalony?
          CData data = sensors->getSensorData(conf->getSensorId());
          EDangerLevel dangerLvl = EDangerLevel::NONE;
          // czy dane z czujnikow sa powyzej danych alarmowych
          if (conf->getAlarmLvl() < data)
          {
            dangerLvl = EDangerLevel::ALARM;
            warningLevel = true;
            // jesli nie dodawac do wektora, uruchom powtornie przeglad i zapisuj do wektora
            if (!addToVector){
              checkOnceAgain = true;
              break;
            }
          }
          else if (conf->getWarnigLvl() < data){
            dangerLvl = EDangerLevel::WARNING;
            warningLevel = true;
            // jesli nie dodawac do wektora, uruchom powtornie przeglad i zapisuj do wektora
            if (!addToVector){
              checkOnceAgain = true;
              break;
            }
          }
          if (addToVector){
            sensorsData.emplace_back(
                  ++idSensorBase,
                              conf->getSensorId(),
                              curTime,
                              ESensorState::OK,
                              dangerLvl,
                              data);
            // skoro dane sa juz dodawane do listy, to nie trzeba powtornie przechodzic
            checkOnceAgain = false;
          }
        }
        else
        {
          if (addToVector){

            sensorsData.emplace_back(
                  ++idSensorBase,
                              conf->getSensorId(),
                              curTime,
                              ESensorState::TURN_OFF,
                              EDangerLevel::NONE,
                              CData());
            // skoro dane sa juz dodawane do listy, to nie trzeba powtornie przechodzic
            checkOnceAgain = false;
          }
        }

      }
      if (checkOnceAgain){
        // uruchom jeszcze raz sprawdzanie danych, ale tym razem zapamietaj
        addToVector = true;
      }
    }while(checkOnceAgain);

    return warningLevel;
  }




}
