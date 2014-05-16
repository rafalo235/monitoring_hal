#ifndef HTTPTHREAD_H
#define HTTPTHREAD_H

#include <QtCore>
#include <QtDebug>
#include <QObject>
#include "protocol.h"
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
namespace NProtocol {

  class CHttpThread : public QObject
  {
    Q_OBJECT
private:
    static QByteArray convertToBinary(const SProtocol& protocol);
    static void convertToBinary(QByteArray& array, const SMonitorData& monitorData);
    static void convertToBinary(QByteArray& array, const SSensorData& sensorData);
    static void convertToBinary(QByteArray& array, const SData& data);

    template<typename T>
    static inline void convertToBinary(QByteArray& array, const T& data);

  public slots:
    void sendData(const SProtocol& protocol) {
      QByteArray postData = convertToBinary(protocol);
      // create custom temporary event loop on stack
          QEventLoop eventLoop;

          // "quit()" the event-loop, when the network request "finished()"
          QNetworkAccessManager mgr;
          QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

          // the HTTP request
          QNetworkRequest req(QUrl("http://localhost:8080/HallMonitorServer/rest/concentrator/post"));
          req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));

          QNetworkReply *reply = mgr.post(req, postData);
          eventLoop.exec(); // blocks stack until "finished()" has been called

          if (reply->error() == QNetworkReply::NoError) {
              //success
              qDebug() << "Success" <<reply->readAll();
              delete reply;
          }
          else {
              //failure
              qDebug() << "Failure" <<reply->errorString();
              delete reply;
          }


      emit resultReady(protocol);
    }

  signals:
    void resultReady(const SProtocol& protocol);
  };
}
#endif // HTTPTHREAD_H
