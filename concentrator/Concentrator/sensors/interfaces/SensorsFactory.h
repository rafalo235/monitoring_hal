#ifndef CSENSORSFACTORY_H
#define CSENSORSFACTORY_H

//#define SENSOR_MOCK

#include <memory>

#include "sensors/interfaces/ISensorsManager.h"
#include "sensors/core/CSensorsManager.h"

#ifdef SENSOR_MOCK
#include "sensors/tests/CSensorsManagerMock.h"
#endif

//! \brief Namespace odpowidzialny za odczyt pomiarow z czujnikow
namespace NSensors{

  //! \brief Smart pointer do manadzera czujnikow
  typedef std::shared_ptr<ISensorsManager> DSensorManager;

  //!
  //! \brief The CSensorFactory class wzorzec fabryki;
  //! tworzy manadzera sensorow, gwarantuje, ze bedzie tylo jeden
  class CSensorFactory
  {

  public:
    CSensorFactory() = delete;
    CSensorFactory& operator=(const CSensorFactory&) = delete;
    CSensorFactory(const CSensorFactory&) = delete;

    //!
    //! \brief getInstance zwraca managera czujnikw
    //! \return smart pointer do manager czujnikow
    static DSensorManager getInstance(){
#ifdef SENSOR_MOCK
       static DSensorManager manager(new CSensorsManagerMock());
#else
       static DSensorManager manager(new CSensorsManager());
#endif

      return manager;
    }
  };
}
#endif // CSENSORSFACTORY_H
