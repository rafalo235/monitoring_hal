#ifndef BYTEWRAPPER_H
#define BYTEWRAPPER_H

#include <QByteArray>
#include <cstring>

namespace NProtocol{

  //! \brief Klasa pomocnicza w konwertowaniu tablicy bajtow na protokol
  class CByteWrapper{

    //! \brief Tablica bajtow
    const char* src;

    //! \brief Wskaznik w tablicy bajtow
    int pointer;

  public:

    //!
    //! \brief CByteWrapper
    //! \param array
    CByteWrapper(const QByteArray& array) : src(array.constData()), pointer(0){

    }

    //! \brief Odczytuje obiekt typu T z tablicy bajtow
    //! \return Odczytany obiekt typu T
    template<typename T>
    T read(){
      // dobrze by bylo zrobic konsturktory rvalue
      T dest;
      memcpy(&dest, src + pointer, sizeof(dest));
      pointer += sizeof(dest);
      return dest;
    }
  };
}

#endif // BYTEWRAPPER_H
