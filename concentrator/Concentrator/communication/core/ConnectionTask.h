#ifndef TASK_H
#define TASK_H

#include <memory>

#include "communication/interfaces/protocol.h"

namespace NProtocol{

  class IConnectionTask;

  //! \brief Smart pointer do zadan wykonywanych przez watek komunikacji
  typedef std::shared_ptr<IConnectionTask> DConnectionTask;

  //!
  //! \brief The IConnectionTask class Klasa abstrakcyjna
  //!  zadan wykonywanych przez watek komunikacji z serwerem.
  //! \author Marcin Serwach
  class IConnectionTask
  {
  public:
    //! \brief Typ zadania - ktora klasa dziedziczy po IConnectionTask.
    enum class EConnectionTaskType{
      SENDING, //!< obiekt typu CSendingTask
      EXIT     //!< obiekt typu CExitTask
    };

  private:
    //! \brief Typ zadania - ktora klasa dziedziczy po IConnectionTask.
    const EConnectionTaskType type;

  public:

    //!
    //! \brief IConnectionTask Konstruktor klasy abstrakcyjnej
    //! \param type1 Typ klasy dziedziczacej.
    //!
    IConnectionTask(const EConnectionTaskType type1) : type(type1)
    {

    }

    //!
    //! \brief getType Zwraca typ zadania
    //! \return typ zadania
    EConnectionTaskType getType() const
    {
      return type;
    }

    //! \brief domyslny dekonstruktor
    virtual ~IConnectionTask() = default;
  };
  // ///////////////////////////////////////////////////////////////

  //! \brief The CSendingTask class Zadanie wysylania danych do serwera
  //! \author Marcin Serwach
  class CSendingTask : public IConnectionTask
  {
  private:
    //! \brief protokol do wyslania
    const CProtocol protocol;

  public:
    //!
    //! \brief CSendingTask Konstruktor
    //! \param protocol1 protokol do wyslania
    //!
    CSendingTask(const CProtocol& protocol1) :
      IConnectionTask(EConnectionTaskType::SENDING),
      protocol(protocol1)
    {

    }

    //!
    //! \brief getProtocol Zawraca protokol do wyslania
    //! \return protokol do wyslania
    const CProtocol getProtocol() const
    {
      return protocol;
    }

  };

// ///////////////////////////////////////////////
  //!
  //! \brief The CExitTask class Zadanie wylaczenia watku komunikacji z serwerem
  class CExitTask : public IConnectionTask
  {
  public:
    //!
    //! \brief CExitTask Konstruktor
    CExitTask() :
      IConnectionTask(EConnectionTaskType::EXIT)
    {

    }

  };
}

#endif // TASK_H
