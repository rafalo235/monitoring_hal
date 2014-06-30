#ifndef ICONNECTIONRESULT_H
#define ICONNECTIONRESULT_H

#include <memory>
#include "communication/interfaces/protocol.h"

namespace NProtocol{

  //! \brief EConnectionStatus  Stan komunikacji HTTP
  enum class EConnectionStatus{
    NONE,                         //!< Dane poprawne
    CONNECTION_ERROR,             //!< Blad polaczenia
    INPUT_PROTOCOL_FORMAT_ERROR,  //!< Niepoprawny format protokolu wysylanego
    OUTPUT_PROTOCOL_FORMAT_ERROR, //!< Niepoprawny format protokolu odebranego
    CRC_ERROR                     //!< Niepoprawny CRC
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
    virtual const CProtocol getSentProtocol() const = 0;

    //!
    //! \brief getReceivedProtocol zwraca protokol odebrany
    //! \return obiekt protokolu odebranego
    virtual const std::shared_ptr<CProtocol> getReceivedProtocol() const = 0;

    //!
    //! \brief getStatus Status wyslanego protokolu
    //! \return status wyslanego protokolu
    virtual EConnectionStatus getStatus() const = 0;

    //! \brief Domyslny dekonstruktor.
    virtual ~IConnectionResult() = default;
  };
}

#endif // ICONNECTIONRESULT_H
