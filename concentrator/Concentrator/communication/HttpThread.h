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
#include "util/Logger.h"

namespace NProtocol {

  //! \brief Stan komunikacji HTTP
  enum class EConnectionStatus{
    NONE,
    CONNECTION_ERROR,
    INPUT_PROTOCOL_FORMAT_ERROR,
    OUTPUT_PROTOCOL_FORMAT_ERROR,

  };

  class CHttpThread : public QObject
  {
    Q_OBJECT
  private:

    //! \brief Konwertuje obiekt protokolu do postaci tablicy binarnej i dodaje do array.
    //!        Nie mozna zrobic prostej konwersji z powodu wykorzystanych unii i dynamicznych tablic
    //! \param[in,out] array tablica bajtow do ktorej ma zostac dodany protokol
    //! \param[in] protocol prokol do konwersji
    //! \return true jesli format protokolu jest prawidlowy, false jesli nie
    static bool convertToBinary(QByteArray& array, const SProtocol& protocol);

    // ponizsze funkcje sluza do konwersji okreslonych struktur - nie chce mi sie opisywac
    inline static void convertToBinary(QByteArray& array, const SMonitorData& monitorData);
    inline static void convertToBinary(QByteArray& array, const SSensorData& sensorData);
    inline static void convertToBinary(QByteArray& array, const SData& data);
    inline static void convertToBinary(QByteArray& array, const SConfigurationResponse& confResp);
    inline static void convertToBinary(QByteArray& array, const SConfiguration& conf);
    inline static void convertToBinary(QByteArray& array, const SConfigurationValue& confValue);
    inline static void convertToBinary(QByteArray& array, const SServerRequest& reqs);
    inline static void convertToBinary(QByteArray& array, const SRequest& req);

    //! \brief Konwertuje dowolny prymityw do tablicy binarnej i dodaje do array
    //! \param[in,out] array tablica bajtow do ktorej maja byc dodane dane z 'data'
    //! \param[in] data dane do konwersji
    template<typename T>
    static inline void convertToBinary(QByteArray& array, const T& data);

    class CByteWrapper{
      const char* src;
      int pointer;
    public:
      CByteWrapper(const QByteArray& array) : src(array.constData()), pointer(0){

      }
      template<typename T>
      T read(){
        // dobrze by bylo zrobic konsturktory rvalue
        T dest;
        memcpy(&dest, src + pointer, sizeof(dest));
        pointer += sizeof(dest);
        return dest;
      }
    };
#define READ_WRAPPER(obj, wrapper) obj = wrapper.read<decltype(obj)>();
    static bool convertToProtocol(SProtocol& protocol, const QByteArray& array);
    inline static bool convertToProtocol(SServerResponse& response, CByteWrapper& wrapper);
    inline static bool convertToProtocol(SConfiguration& configuration, CByteWrapper& wrapper);
    inline static bool convertToProtocol(SConfigurationValue& confValue, CByteWrapper& wrapper);
    inline static bool convertToProtocol(SData& sdata, CByteWrapper& wrapper);

  public slots:
    void sendData(const SProtocol& protocol);

  signals:
    void resultReady(const EConnectionStatus error, const SProtocol& protocol);
  };
}
#endif // HTTPTHREAD_H
