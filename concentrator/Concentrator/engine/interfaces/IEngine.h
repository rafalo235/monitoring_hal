#ifndef IENGINE_H
#define IENGINE_H

namespace NEngine {
  //!
  //! \brief The IEngine class klasa abstrakcyjna silnika
  //! \author Marcin Serwach
  class IEngine
  {
  public:

    //!
    //! \brief init inicjuje wszystko (wczytuje dane konfiguracyjne, wlacza watki)
    //! \return true jesli poprawnie zainicjalizowano silnik
    virtual bool init() = 0;

    //!
    //! \brief exit Wylacza watki.
    virtual void exit() = 0;

    virtual ~IEngine() = default;
  };
}


#endif // IENGINE_H
