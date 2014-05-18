#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "HttpThread.h"
#include <QDebug>
#include <memory>
#include "communication/protocol.h"

namespace NProtocol {
  class CCommunication;
    typedef std::shared_ptr<CCommunication> DCommunication;
  class CCommunication : public QObject
  {
    Q_OBJECT
    QThread workerThread;

    static decltype(SProtocol::idPackage) idPackageBase;

public:

 private:
    static DCommunication obj;
    CCommunication();
    CCommunication(const CCommunication&) = delete;
    CCommunication& operator=(const CCommunication&) = delete;

    decltype(SProtocol::idPackage) sendProtocol(const EMessageType type, const UMessage& message);

  public:

    static DCommunication getInstance(){
      if (obj.use_count() == 0){
        obj.reset(new CCommunication());
      }
      return obj;
    }

    ~CCommunication();

    //! \brief Wysyla dane z czujnikow do serwera
    //! \param[in] data Dane czujnikow
    //! \return Id pakietu
    decltype(SProtocol::idPackage) sendMonitorData(const SMonitorData& data);

    //! \brief Wysyla potwierdzenie zmiany konfiguracji do serwera
    //! \param[in] data Aktualna konfiguracja
    //! \return Id pakietu
    decltype(SProtocol::idPackage) sendConfigurationResponse(const SConfigurationResponse& data);

    //! \brief Wysyla prosbe o wyslanie konfigruacji do serwera
    //! \param[in] data Typy konfiguracji
    //! \return Id pakietu
    decltype(SProtocol::idPackage) sendServerRequest(const SServerRequest& data);


  public slots:
    void handleResults(const EConnectionStatus error, const SProtocol& res);


  signals:
    void sendProtocol(const uint32_t, const EMessageType, const UMessage&);
  };
}
#endif // COMMUNICATION_H
