#ifndef TASK_H
#define TASK_H

#include <memory>

#include "communication/interfaces/protocol.h"

namespace NProtocol{

  class IConnectionTask;
  typedef std::shared_ptr<IConnectionTask> DConnectionTask;

  class IConnectionTask
  {
  public:
    enum class EConnectionTaskType{SENDING, EXIT};
  private:
    const EConnectionTaskType type;
  public:

    IConnectionTask(const EConnectionTaskType type1) : type(type1)
    {

    }

    EConnectionTaskType getType() const
    {
      return type;
    }

    virtual ~IConnectionTask(){}
  };

  class CSendingTask : public IConnectionTask
  {
  private:
    const CProtocol protocol;
  public:
    CSendingTask(const CProtocol& protocol1) :
      IConnectionTask(EConnectionTaskType::SENDING),
      protocol(protocol1)
    {

    }

    const CProtocol getProtocol() const
    {
      return protocol;
    }
    virtual ~CSendingTask(){}
  };

  class CExitTask : public IConnectionTask
  {
  public:
    CExitTask() :
      IConnectionTask(EConnectionTaskType::EXIT)
    {}
    virtual ~CExitTask(){}
  };
}

#endif // TASK_H
