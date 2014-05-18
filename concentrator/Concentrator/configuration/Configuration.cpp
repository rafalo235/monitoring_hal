#include "Configuration.h"
#include <algorithm>

namespace NEngine {

  //!  \brief Odczyt konfiguracji z pliku konfiguracyjnego
  std::ifstream& operator >>(std::ifstream& stream, CConfiguration& obj)
  {
    stream>>obj.idConcentrator;
    stream>>obj.sendingPeriod;
    std::vector<CSensorConfiguration>::size_type size;
    stream>>size;

    CSensorConfiguration sensorConf;
    for(decltype(size) i = 0; i < size; ++i){
      stream>>sensorConf;
      obj.sensors.push_back(sensorConf);
    }

    return stream;
  }

  std::ofstream& operator <<(std::ofstream& stream, const CConfiguration& obj)
  {
    stream<<obj.idConcentrator;
    stream<<obj.sendingPeriod;
    stream<<obj.sensors.size();
    std::for_each(obj.sensors.begin(), obj.sensors.end(),
                  [&](const CSensorConfiguration& conf){ stream<<conf; });

    return stream;
  }

}
