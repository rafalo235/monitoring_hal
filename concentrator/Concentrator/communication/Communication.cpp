#include "Communication.h"

namespace NProtocol{
  CCommunication::CCommunication() {
    qRegisterMetaType<SProtocol>("SProtocol");
    qRegisterMetaType<EConnectionStatus>("EConnectionStatus");

    CHttpThread *worker = new CHttpThread;
    worker->moveToThread(&workerThread);

    connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(sendProtocol(SProtocol)), worker, SLOT(sendData(SProtocol)));
    connect(worker, SIGNAL(resultReady(EConnectionStatus, SProtocol)), this, SLOT(handleResults(EConnectionStatus, SProtocol)));

    workerThread.start();

  }

  CCommunication::~CCommunication() {
    workerThread.quit();
    workerThread.wait();
  }
  void CCommunication::handleResults(const EConnectionStatus error, const SProtocol& protocol)
  {
    LOG_DEBUG("Sending result: ", static_cast<int>(error));
    LOG_PROTOCOL(protocol);
  }

}
