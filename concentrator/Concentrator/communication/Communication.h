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

  public:

    static DCommunication getInstance(){
      if (obj.use_count() == 0){
        obj.reset(new CCommunication());
      }
      return obj;
    }

    ~CCommunication();

  decltype(SProtocol::idPackage) sendProtocol(const EMessageType type, const UMessage& message);

  public slots:
    void handleResults(const EConnectionStatus error, const SProtocol& res);


  signals:
    void sendProtocol(const uint32_t, const EMessageType, const UMessage&);
  };
}
#endif // COMMUNICATION_H
