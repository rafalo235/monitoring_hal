#ifndef CONNECTIONRESULT_H
#define CONNECTIONRESULT_H

#include <memory>

#include "communication/interfaces/protocol.h"
#include "communication/interfaces/IConnectionResult.h"

namespace NProtocol{

  //! \brief Resultat wysylanego (lub nie) protokolu z danymi
  class CConnectionResult : public IConnectionResult
  {
  private:

    const CProtocol sentProtocol;
    const std::shared_ptr<CProtocol> receivedProtocol;
    const EConnectionStatus status;

  public:

    CConnectionResult(const CProtocol& sentProtocol1,
                      const std::shared_ptr<CProtocol>& receivedProtocol1,
                      const EConnectionStatus status1) noexcept:
                        sentProtocol(sentProtocol1),
                        receivedProtocol(receivedProtocol1),
                        status(status1)
    {

    }

    CConnectionResult(const CProtocol& sentProtocol1,
                      const EConnectionStatus status1) noexcept:
                        sentProtocol(sentProtocol1),
                        status(status1)
    {

    }

    virtual const CProtocol getSentProtocol() const noexcept
    {
      return sentProtocol;
    }

    virtual const std::shared_ptr<CProtocol> getReceivedProtocol() const noexcept
    {
      return receivedProtocol;
    }

    virtual EConnectionStatus getStatus() const noexcept
    {
      return status;
    }
  };
}
#endif // CONNECTIONRESULT_H
