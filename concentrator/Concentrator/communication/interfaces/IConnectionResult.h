#ifndef ICONNECTIONRESULT_H
#define ICONNECTIONRESULT_H

#include <memory>
#include "communication/interfaces/protocol.h"

namespace NProtocol{
  //! \brief EConnectionStatus  Stan komunikacji HTTP
  enum class EConnectionStatus{
    NONE,
    CONNECTION_ERROR,
    INPUT_PROTOCOL_FORMAT_ERROR,
    OUTPUT_PROTOCOL_FORMAT_ERROR,

  };

  //!
  //! \brief The IConnectionResult class jest interfejsem danych otrzymanych z serwera lub danych niewyslanych.
  //!
  class IConnectionResult
  {
  public:

    //!
    //! \brief getSentProtocol zwraca protokol wysyłany do serwera
    //! \return obiekt protokolu wysylanego
    //!
    virtual const SProtocol getSentProtocol() const = 0;

    //!
    //! \brief getReceivedProtocol zwraca protokol odebrany lub wysylany jesli nie zostal wyslany
    //! \return obiekt protokolu odebranego lub wysylanego
    //!
    virtual const SProtocol getReceivedProtocol() const = 0;

    //!
    //! \brief getStatus Status wyslanego protokolu
    //! \return status wyslanego protokolu
    //!
    virtual EConnectionStatus getStatus() const = 0;

    virtual ~IConnectionResult() = default;
  };
}

#endif // ICONNECTIONRESULT_H
