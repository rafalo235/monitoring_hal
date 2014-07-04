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

  //!
  //! \brief The CHttpThread class Klasa, w ktorej dziala watek komunikacji z serwerem
  //! \author Marcin Serwach
  class CHttpThread
  {
  private:

    //! \brief watek komunikacji
    std::shared_ptr<std::thread> thread;

    //! \brief Kolejka zadania do wykonania
    NUtil::CQueueThread<DConnectionTask> sendingQueue;

    //! \brief Kolejka rezultatow watku.
    NUtil::CQueueThread<DConnectionResult> resultsQueue;

    //! \brief Konwertuje obiekt protokolu do postaci tablicy bajtow i dodaje do array.
    //!        Nie mozna zrobic prostej konwersji z powodu wykorzystanych unii
    //! \param[in,out] array tablica bajtow do ktorej ma zostac dodany protokol
    //! \param[in] protocol prokol do konwersji
    //! \return true jesli format protokolu jest prawidlowy, false jesli nie
    static bool convertToBinary(QByteArray& array, const CProtocol& protocol);

    //!
    //! \brief convertToBinary Konwertuj wiadomosc do postaci tablicy bajtow
    //! \param array[in,out] array tablica bajtow, do ktorej ma zostac dodana wiadomosc
    //! \param monitorData smart pointer do wiadomosci
    inline static void convertToBinary(QByteArray& array, const std::shared_ptr<CMonitorData>& monitorData);

    //!
    //! \brief convertToBinary Konwertuje dane z czujnikow do tablic bajtow
    //! \param array[in,out] array tablica bajtow, do ktorej maja zostac zapisane dane z czujnikow
    //! \param sensorData dane z czujnikow
    inline static void convertToBinary(QByteArray& array, const CSensorData& sensorData);

    //!
    //! \brief convertToBinary Konwertuje dane do tablic bajtow
    //! \param array[in,out] array tablica bajtow ,do ktorej ma zostac dodane dane
    //! \param data dane
    inline static void convertToBinary(QByteArray& array, const CData& data);

    //!
    //! \brief convertToBinary Konwertuje potwierdzenie konfiguracji do tablic bajtow
    //! \param array[in,out] array tablica bajtow, do ktorej ma zostac dodane potwierdzenie konfiguracji
    //! \param confResp potwierdzenie konfiguracji
    inline static void convertToBinary(QByteArray& array, const std::shared_ptr<CConfigurationResponse>& confResp);

    //!
    //! \brief convertToBinary Konwertuje konfiguracje do tablic bajtow
    //! \param array[in,out] array tablica bajtow do ktorej ma zostac dodana konfiguracja
    //! \param conf konfiguracja
    inline static void convertToBinary(QByteArray& array, const CConfiguration& conf);

    //!
    //! \brief convertToBinary Konwertuje wartosci konfiguracji do tablic bajtow
    //! \param array[in,out] array tablica bajtow do ktorej ma zostac dodana wartosc konf
    //! \param confValue wartosci konfiguracji
    inline static void convertToBinary(QByteArray& array, const CConfigurationValue& confValue);

    //!
    //! \brief convertToBinary Konwertuje prosbe konfiguracji do tablic bajtow
    //! \param array[in,out] array tablica bajtow do ktorej ma zostac dodana prosba konfiguracji
    //! \param reqs prosba konfiguracji
    inline static void convertToBinary(QByteArray& array, const std::shared_ptr<CServerRequest>& reqs);

    //!
    //! \brief convertToBinary Konwertuje pojedyncza prosba konfiguracji do tablic bajtow
    //! \param array[in,out] array tablica bajtow do ktorej ma zostac dodana pojedyncza prosba
    //! \param req pojedyncza prosba konfiguracji
    inline static void convertToBinary(QByteArray& array, const CRequest& req);

    //! \brief Konwertuje dowolny prymityw do tablicy binarnej i dodaje do array
    //! \param[in,out] array tablica bajtow do ktorej maja byc dodane dane z 'data'
    //! \param[in] data dane do konwersji
    template<typename T>
    static inline void convertToBinary(QByteArray& array, const T& data);

    //! \brief Konwertuje bajty na obiekty/prymityw
    //! \param[in] wrapper bajty
    //! \return odczytany obiekt
    template<typename T>
    static inline T convertToProtocol(CByteWrapper& wrapper);

    //! \brief convertToPrototocl Konwertuje tablice bajtow do protokolu
    //! \param[in] wrapper Tablica bajtow
    //! \return smart pointer; jesli protokol ma poprawna strukture to smart pointer ma dynamicznie
    //!         zalokowana pamiec
    static std::shared_ptr<CProtocol> convertToProtocol(CByteWrapper& wrapper);

    //! \brief convertToProtocol Konwertuj tablic bajtow do wiadomosci typu CServerResponse
    //! \param message[out] odczytana odpowiedz z serwera
    //! \param wrapper tablica bajtow
    //! \return true jesli konwersja poprawna; false jesli nie
    static bool convertToProtocolServerReponse(std::shared_ptr<IMessage>& message, CByteWrapper& wrapper);

    //! \brief convertToProtocol Konwertuj tablic bajtow do wiadomosci typu CMonitorData
    //! \param message[out] odczytana odpowiedz z serwera
    //! \param wrapper tablica bajtow
    //! \return true jesli konwersja poprawna; false jesli nie
    static bool convertToProtocolMonitorData(std::shared_ptr<IMessage>& message, CByteWrapper& wrapper);

    //! \brief convertToProtocol Konwertuj tablic bajtow do wiadomosci typu CConfigurationResponse
    //! \param message[out] odczytana odpowiedz z serwera
    //! \param wrapper tablica bajtow
    //! \return true jesli konwersja poprawna; false jesli nie
    static bool convertToProtocolConfigurationResponse(std::shared_ptr<IMessage>& message, CByteWrapper& wrapper);

    //! \brief convertToProtocol Konwertuj tablic bajtow do wiadomosci typu CServerRequest
    //! \param message[out] odczytana odpowiedz z serwera
    //! \param wrapper tablica bajtow
    //! \return true jesli konwersja poprawna; false jesli nie
    static bool convertToProtocolServerRequest(std::shared_ptr<IMessage>& message, CByteWrapper& wrapper);
    //!
    //! \brief convertToProtocol Konwertuje tablice bajtow na konfiguracje
    //! \param configurations odczytana konfiguracja
    //! \param wrapper tablica bajtow
    //! \return true jesli konwersja poprawna; false jesli nie
    static bool convertToProtocol(std::vector<CConfigurationValue>& configurations, CByteWrapper& wrapper);

    //!
    //! \brief convertToProtocol Konwertuje tablice bajtow na dane
    //! \param sdata odczytane dane
    //! \param wrapper tablica bajtow
    //! \return true jesli konwersja poprawna; false jesli nie
    static bool convertToProtocol(CData& sdata, CByteWrapper& wrapper);
    // ////////////////////////////////////////////////////////////////////

    //!
    //! \brief convertToProtocolDebug wyswietla protokol
    //! \param array bajty
    static void convertToProtocolDebug(const QByteArray& array);
  public:
    //!
    //! \brief CHttpThread Konstruktor, ktory uruchamia watek komunikacji
    CHttpThread();

    //!
    //! \brief addToSendingQueue Dodaje zadanie do kolejki, ktorym jest wyslanie protokolu
    //! \param prot Protokol do wyslania
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

    //!
    //! \brief run Glowna funkcja watku komunikacji
    void run();

    //!
    //! \brief sendHttp Wyslanie danych do serwera i oczekiwania na odpowiedz
    //! \param protocol protokol do wyslania
    void sendHttp(const CProtocol& protocol);

  };
}
#endif // HTTPTHREAD_H
