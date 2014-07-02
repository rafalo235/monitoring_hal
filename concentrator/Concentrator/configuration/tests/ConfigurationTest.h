#ifndef CCONFIGURATIONTEST_H
#define CCONFIGURATIONTEST_H

#ifdef TEST_ENABLE
#ifdef TEST_ENABLE_CONFIGURATION_TEST
#include "ConfigurationMock.h"
#include "util/test/Test.h"

namespace NTest
{
  using namespace NProtocol;
  using namespace NEngine;

  class CConfigurationTest : public CTest<CConfigurationTest>
  {
  public:
    CConfigurationTest()
    {
      addTestCase("saveReadTest", &CConfigurationTest::saveReadTest);
    }

    void saveReadTest()
    {
      CConfigurationMock conf;
      const int idSensor = 0;
      int alarmValue = 40;
      int warningValue = 10;
      CData alarmNew(EValueType::INT_32, &alarmValue);
      CData warningNew(EValueType::INT_32, &warningValue);

      conf.setAlarmLevel(idSensor, alarmNew);
      conf.setWarningLevel(idSensor, warningNew);

      const std::vector<DSensorConfiguration> read = conf.getSensorConfiguration();
      std::vector<DSensorConfiguration>::const_iterator it =
          std::find_if(read.begin(), read.end(),
                       [&](const DSensorConfiguration& d){ return idSensor == d->getSensorId(); });

      CData alarmNewRead = (*it)->getAlarmLvl();
      CData warningNewRead = (*it)->getWarnigLvl();

      assertEq(alarmNew.getValue().vInt32, alarmNewRead.getValue().vInt32);
      assertEq(warningNew.getValue().vInt32, warningNewRead.getValue().vInt32);

    }
  };
}
#endif // TEST_ENABLE_CONFIGURATION_TEST
#endif // TEST_ENABLE
#endif // CCONFIGURATIONTEST_H
