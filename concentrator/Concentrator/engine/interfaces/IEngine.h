#ifndef IENGINE_H
#define IENGINE_H

namespace NEngine {
  class IEngine
  {
  public:

    virtual bool init() = 0;
    virtual void exit() = 0;

    virtual ~IEngine() = default;
  };
}


#endif // IENGINE_H
