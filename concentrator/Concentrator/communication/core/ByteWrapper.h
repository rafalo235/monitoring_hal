#ifndef BYTEWRAPPER_H
#define BYTEWRAPPER_H

#include <QByteArray>
#include <cstring>
#include "util/Cryptography.h"
#include "util/Memory.h"

#include "util/Logger.h"

namespace NProtocol{

  //! \brief Klasa pomocnicza w konwertowaniu tablicy bajtow na protokol
  class CByteWrapper{

    //! \brief Tablica bajtow
    std::shared_ptr<char> src;

    //! \brief Wskaznik w tablicy bajtow
    int pointer;

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

    void display() const
    {
      LOG_DEBUG("siz bytes: ", size);
      char* buf = &*src;
      for (int i = 0; i < size; ++i)
      {
        std::cout<<(int)*(buf + i)<<std::endl;
      }
    }
    int getSize() const
    {
      return size;
    }

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
