#include "Time.h"

namespace NUtil
{
#ifdef TEST_ENABLE
  long CTime::secondsMoved;
#endif

  //!
  //! \brief operator << przeciazony operator daty
  //! \param stream strumien
  //! \param time czas
  //! \return  strumien
  std::ostream& operator<<(std::ostream& stream, const STime& time)
  {
    stream<<time.getHour()<<":"<<time.getMinute()<<":"<<time.getSecond()<<" "
         <<time.getDay()<<"."<<time.getMonth()<<"."<<time.getYear();
    return stream;
  }
}
