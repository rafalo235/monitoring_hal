#ifndef TASK_H
#define TASK_H

#include "communication/interfaces/protocol.h"

namespace NProtocol{
  class CConnectionTask
  {
  private:
    const SProtocol* protocol;
  public:
    CConnectionTask() : protocol(nullptr){

    }

    CConnectionTask(const SProtocol& protocol1) : protocol(&protocol1){

    }

    const SProtocol* getProtocol() const{
      return protocol;
    }
  };
}

#endif // TASK_H
