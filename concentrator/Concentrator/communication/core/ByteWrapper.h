#ifndef BYTEWRAPPER_H
#define BYTEWRAPPER_H

#include <QByteArray>
#include <cstring>
#include "util/Cryptography.h"
#include "util/Memory.h"

#include "util/Logger.h"

namespace NProtocol{

  //! \brief Klasa pomocnicza w konwertowaniu tablicy bajtow na protokol
  //! \author Marcin Serwach
  class CByteWrapper{

    //! \brief Tablica bajtow
    std::shared_ptr<char> src;

    //! \brief Wskaznik w tablicy bajtow
    int pointer;

    //! \brief rozmiar danych
    int size;

  public:

    //!
    //! \brief CByteWrapper
    //! \param array
    CByteWrapper(const QByteArray& array) : pointer(0), size(array.size()){

      src = NUtil::CMemory::getArray<char>(size);
      char* buf = &*src;
      memcpy(buf, array.constData(), size);

    }

    //!
    //! \brief display wyswietla bajty
    void display() const
    {
      LOG_DEBUG("size bytes: ", size);
      char* buf = &*src;
      for (int i = 0; i < size; ++i)
      {
        std::cout<<(int)*(buf + i)<<std::endl;
      }
    }

    //!
    //! \brief getSize zwraca rozmiar
    //! \return rozmiar
    int getSize() const
    {
      return size;
    }

    //!
    //! \brief isCRCValid sprawdza crc, przyjmuj ze CRC zajmuje ostatnie bajty
    //! \return true jesli CRC jest poprawny
    bool isCRCValid()
    {
      char* buf = &*src;
      return NUtil::CCryptography::checkCrc16(buf, size);
    }

    //! \brief Odczytuje obiekt typu T z tablicy bajtow
    //! \return Odczytany obiekt typu T
    template<typename T>
    T read(){
      // dobrze by bylo zrobic konsturktory rvalue
      T dest;
      char* buf = &*src;
      memcpy(&dest, buf + pointer, sizeof(dest));
      pointer += sizeof(dest);
      return dest;
    }

    void movePosition(int move)
    {
      pointer += move;
    }
  };
}

#endif // BYTEWRAPPER_H
