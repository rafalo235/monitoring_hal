#ifndef MEMORY_H
#define MEMORY_H
#include <memory>

namespace NUtil
{
  //!
  //! \brief The CMemory class klasa pomocna w tworzeniu smart pointer do tablic
  //! \author Marcin Serwach
  class CMemory
  {
  public:
    CMemory() = delete;

    //!
    //! \brief getArray Funkcja zwraca smart pointera do dynamicznej tablicy typow T2
    //! \param[in] size rozmian dynamicznej tablicy podanych w ilosci typow T2
    //! \return smart pointer do tablicy
    template<typename T2>
    inline static std::shared_ptr<T2> getArray(int size)
    {
      std::shared_ptr<T2> p(new T2[size], [](T2* a)
      { delete[] a;});
      return p;
    }
  };
}
#endif // MEMORY_H
