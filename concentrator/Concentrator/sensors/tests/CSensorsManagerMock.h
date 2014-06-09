#ifndef CSENSORSMANAGERMOCK_H
#define CSENSORSMANAGERMOCK_H
#define SENSOR_MOCK
#include "sensors/core/CSensorsManager.h"
#include "communication/interfaces/protocol.h"
#include "util/Logger.h"

namespace NSensors{
  //!
  //! \brief The CSensorsManagerMock class Mock manadzera czujnikow
  class CSensorsManagerMock : public CSensorsManager
  {
  public:
    //!
    //! \brief CSensorsManagerMock Konstruktor
    CSensorsManagerMock()
    {
      LOG_DEBUG("CSensorsManagerMock - constructor.");
    }

    virtual bool getSensorData(const NEngine::DSensorConfiguration& conf, CData& value) const{
      int buf = 4;
      value.setValue(EValueType::INT_32, &buf);
      return true;
    }
  };
}
#endif // CSENSORSMANAGERMOCK_H
