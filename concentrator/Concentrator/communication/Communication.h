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
    CCommunication() {
      qRegisterMetaType<SProtocol>("SProtocol");

      CHttpThread *worker = new CHttpThread;
      worker->moveToThread(&workerThread);

      connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
      connect(this, SIGNAL(sendProtocol(SProtocol)), worker, SLOT(sendData(SProtocol)));
      connect(worker, SIGNAL(resultReady(SProtocol)), this, SLOT(handleResults(SProtocol)));

      workerThread.start();
      qDebug() <<"Controller2 thread: "<<QThread::currentThreadId();
    }

    ~CCommunication() {
      workerThread.quit();
      workerThread.wait();
    }

  public slots:
    void handleResults(const SProtocol& res)
    {
      qDebug() << "handle results: "<<res.version<<" thread: "<<QThread::currentThreadId();
    }

  signals:
    void sendProtocol(const SProtocol&);
  };
}
#endif // COMMUNICATION_H
