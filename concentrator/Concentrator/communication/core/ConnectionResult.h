#ifndef CONNECTIONRESULT_H
#define CONNECTIONRESULT_H

#include "communication/interfaces/protocol.h"
#include "communication/interfaces/IConnectionResult.h"

namespace NProtocol{

  //! \brief Resultat wysylanego (lub nie) protokolu z danymi
  class CConnectionResult : public IConnectionResult
  {
  private:

    const SProtocol sentProtocol;
    const SProtocol receivedProtocol;
    const EConnectionStatus status;

  public:

    CConnectionResult(const SProtocol& sentProtocol1,
                      const SProtocol& receivedProtocol1,
                      const EConnectionStatus status1) :
      sentProtocol(sentProtocol1), receivedProtocol(receivedProtocol1), status(status1){

    }

    virtual const SProtocol getSentProtocol() const{
      return sentProtocol;
    }

    virtual const SProtocol getReceivedProtocol() const{
      return receivedProtocol;
    }

    virtual EConnectionStatus getStatus() const{
      return status;
    }
  };
}
#endif // CONNECTIONRESULT_H
