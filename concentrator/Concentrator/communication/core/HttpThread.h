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
    NUtil::CQueueThread<DConnectionTask> sendingQueue;
    NUtil::CQueueThread<DConnectionResult> resultsQueue;

    //! \brief Konwertuje obiekt protokolu do postaci tablicy binarnej i dodaje do array.
    //!        Nie mozna zrobic prostej konwersji z powodu wykorzystanych unii i dynamicznych tablic
    //! \param[in,out] array tablica bajtow do ktorej ma zostac dodany protokol
    //! \param[in] protocol prokol do konwersji
    //! \return true jesli format protokolu jest prawidlowy, false jesli nie
    static bool convertToBinary(QByteArray& array, const CProtocol& protocol);

    // ponizsze funkcje sluza do konwersji okreslonych struktur - nie chce mi sie opisywac
    inline static void convertToBinary(QByteArray& array, const std::shared_ptr<CMonitorData>& monitorData);
    inline static void convertToBinary(QByteArray& array, const CSensorData& sensorData);
    inline static void convertToBinary(QByteArray& array, const CData& data);
    inline static void convertToBinary(QByteArray& array, const std::shared_ptr<CConfigurationResponse>& confResp);
    inline static void convertToBinary(QByteArray& array, const CConfiguration& conf);
    inline static void convertToBinary(QByteArray& array, const CConfigurationValue& confValue);
    inline static void convertToBinary(QByteArray& array, const std::shared_ptr<CServerRequest>& reqs);
    inline static void convertToBinary(QByteArray& array, const CRequest& req);

    //! \brief Konwertuje dowolny prymityw do tablicy binarnej i dodaje do array
    //! \param[in,out] array tablica bajtow do ktorej maja byc dodane dane z 'data'
    //! \param[in] data dane do konwersji
    template<typename T>
    static inline void convertToBinary(QByteArray& array, const T& data);

    //! \brief makro do ulatwienia odczytu
#define READ_WRAPPER(obj, wrapper) obj = wrapper.read<decltype(obj)>();

    std::shared_ptr<CProtocol> convertToProtocol(CByteWrapper& wrapper);
    bool convertToProtocol(std::shared_ptr<IMessage>& message, CByteWrapper& wrapper);
    bool convertToProtocol(std::vector<CConfigurationValue>& configurations, CByteWrapper& wrapper);
    bool convertToProtocol(CData& sdata, CByteWrapper& wrapper);
    // ////////////////////////////////////////////////////////////////////

  public:
    CHttpThread();

    void addToSendingQueue(const CProtocol& prot);

    //! IConnection::getResult()
    const DConnectionResult getResult() {
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

    void sendHttp(const CProtocol& protocol);



  };
}
#endif // HTTPTHREAD_H
