#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "HttpThread.h"
#include <QDebug>


namespace NProtocol {
  class CCommunication : public QObject
  {
    Q_OBJECT
    QThread workerThread;
  public:
    CCommunication();

    ~CCommunication();

  public slots:
    void handleResults(const EConnectionStatus error, const SProtocol& res);

  signals:
    void sendProtocol(const SProtocol&);
  };
}
#endif // COMMUNICATION_H
