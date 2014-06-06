#ifndef SENSORCONFIGURATIONMOCK_H
#define SENSORCONFIGURATIONMOCK_H

#include "configuration/core/SensorConfiguration.h"

namespace NEngine{

  //!
  //! \brief The CSensorConfigurationMock class Mock konfiguracja pojedynczego sensora
  class CSensorConfigurationMock : public CSensorConfiguration
  {

  public:

    //!
    //! \brief CSensorConfigurationMock Konstruktor
    //! \param idSensor1 id czujnika
    //! \param turnOn1 wlaczony/wylaczony
    //! \param warningLvl1 poziom ostrzegania
    //! \param alarmLvl1 poziom alarmowy
    CSensorConfigurationMock(uint8_t& idSensor1,
                             bool& turnOn1,
                             CData& warningLvl1,
                         CData& alarmLvl1,
                         std::string& ipv4Address1,
                         int& port1) :
                    CSensorConfiguration(idSensor1, turnOn1, warningLvl1, alarmLvl1, ipv4Address1, port1)
    {

    }
  };

}
#endif // SENSORCONFIGURATIONMOCK_H
