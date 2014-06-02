#ifndef HTTPTHREAD_H
#define HTTPTHREAD_H

#include <thread>
#include <QByteArray>
#include <memory>

#include "util/QueueThread.h"
#include "communication/interfaces/protocol.h"
#include "communication/core/ByteWrapper.h"
#include "communication/core/ConnectionResult.h"
#include "communication/core/ConnectionTask.h"
#include "communication/interfaces/IConnection.h"

namespace NProtocol {

  class CHttpThread
  {
  private:

    std::shared_ptr<std::thread> thread;
    NUtil::CQueueThread<CConnectionTask> sendingQueue;
    NUtil::CQueueThread<DConnectionResult> resultsQueue;

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

    //! \brief makro do ulatwienia odczytu
#define READ_WRAPPER(obj, wrapper) obj = wrapper.read<decltype(obj)>();

    //! \brief Konwertuje tablice bajtow na protokol
    //! \param[in,out] protocol protokol odczytany
    //! \param[in] array tablica bajtow
    //! \return true jesli wszystko ok; false jesli byl blad formatu protokolu
    static bool convertToProtocol(SProtocol& protocol, const QByteArray& array);


    inline static bool convertToProtocol(SServerResponse& response, CByteWrapper& wrapper);
    inline static bool convertToProtocol(SConfiguration& configuration, CByteWrapper& wrapper);
    inline static bool convertToProtocol(SConfigurationValue& confValue, CByteWrapper& wrapper);
    inline static bool convertToProtocol(SData& sdata, CByteWrapper& wrapper);

    // ////////////////////////////////////////////////////////////////////

  public:
    CHttpThread();

    void addToSendingQueue(const SProtocol& prot);

    //! IConnection::getResult()
    const DConnectionResult getResult(){
      return resultsQueue.pop();
    }

    //! IConnection::exit()
    void exit();

    //! IConnection::isAnyResult()
    bool isAnyResult() const{
      return resultsQueue.size() != 0;
    }

  private:
    void run();

    void sendHttp(const SProtocol& protocol);



  };
}
#endif // HTTPTHREAD_H
