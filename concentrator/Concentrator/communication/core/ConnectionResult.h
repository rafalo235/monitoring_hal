#ifndef CONNECTIONRESULT_H
#define CONNECTIONRESULT_H

#include <memory>

#include "communication/interfaces/protocol.h"
#include "communication/interfaces/IConnectionResult.h"

namespace NProtocol{

  //!
  //! \brief The CConnectionResult class Dane otrzymane z serwera lub ewentualne bledy.
  //! \author Marcin Serwach
  class CConnectionResult : public IConnectionResult
  {
  private:

    //! \brief Wyslany pakiet
    const CProtocol sentProtocol;

    //! \brief Otrzymany pakiet
    const std::shared_ptr<CProtocol> receivedProtocol;

    //! \brief Status wyslanego (lub nie) pakietu
    const EConnectionStatus status;

  public:

    //!
    //! \brief CConnectionResult Konstruktor dla otrzymanego pakietu z serwera
    //! \param sentProtocol1 wysylany protokol
    //! \param receivedProtocol1 otrzymany protokol
    //! \param status1 status komunikacji
    //!
    CConnectionResult(const CProtocol& sentProtocol1,
                      const std::shared_ptr<CProtocol>& receivedProtocol1,
                      const EConnectionStatus status1) noexcept:
                        sentProtocol(sentProtocol1),
                        receivedProtocol(receivedProtocol1),
                        status(status1)
    {

    }

    //!
    //! \brief CConnectionResult Konstruktor dla nie wyslanych danych
    //! \param sentProtocol1 wysylany protokol
    //! \param status1 status komunikacji
    //!
    CConnectionResult(const CProtocol& sentProtocol1,
                      const EConnectionStatus status1) noexcept:
                        sentProtocol(sentProtocol1),
                        status(status1)
    {

    }

    //! IConnectionResult::getSentProtocol()
    virtual const CProtocol getSentProtocol() const noexcept
    {
      return sentProtocol;
    }

    //! IConnectionResult::getReceivedProtocol()
    virtual const std::shared_ptr<CProtocol> getReceivedProtocol() const noexcept
    {
      return receivedProtocol;
    }

    //! IConnectionResult::getStatus()
    virtual EConnectionStatus getStatus() const noexcept
    {
      return status;
    }
  };
}
#endif // CONNECTIONRESULT_H
