#ifndef CTIME_H
#define CTIME_H
#include <chrono>
#include <ctime>
#include <ratio>
#include <iostream>


namespace NUtil
{

  //! \brief Sktruktura czasu
  //! \author Marcin Serwach
  class STime
  {
    //! \brief czas
    long time;
    //! \brief rok
    int year;
    //! \brief miesiac
    int month;
    //! \brief dzien
    int day;
    //! \brief godzina format 24
    int hour;
    //! \brief minutya
    int minute;
    //! \brief sekunda
    int second;

  public:
    //! \brief STime domyslny konstruktor
    STime() : time(0), year(0), month(0), day(0), hour(0), minute(0), second(){}

    //!
    //! \brief STime konstruktor
    //! \param timeStd1 czas z std
    //! \param year1 rok
    //! \param month1 miesiac
    //! \param day1 dzien
    //! \param hour1 godzina
    //! \param minute1 minuty
    //! \param second1 sekondy
    STime(const long timeStd1,
          const int year1,
          const int month1,
          const int day1,
          const int hour1,
          const int minute1,
          const int second1) :
            time(timeStd1),
            year(year1),
            month(month1),
            day(day1),
            hour(hour1),
            minute(minute1),
            second(second1)
      {}

    //! \brief STime konstruktor
    //! \param timeStd1 czas zgodny z SDT
    //! \param time1 struktura czasu z std
    STime(long timeStd1, const tm* time1) :
      time(timeStd1),
      year(time1->tm_year + 1900),
      month(time1->tm_mon + 1),
      day(time1->tm_mday),
      hour(time1->tm_hour),
      minute(time1->tm_min),
      second(time1->tm_sec)
    {}

    //! \brief STime konstruktor
    //! \param year1 rok
    //! \param month1 miesiac
    //! \param day1 dzien
    //! \param hour1 godzina
    //! \param minute1 minuty
    //! \param second1 sekondy
    STime(const int year1,
          const int month1,
          const int day1,
          const int hour1,
          const int minute1,
          const int second1)
      :
        year(year1),
        month(month1),
        day(day1),
        hour(hour1),
        minute(minute1),
        second(second1)
    {
      struct tm  tm;
      time_t rawtime;
      std::time ( &rawtime );
      tm = *localtime ( &rawtime );
      tm.tm_year = year - 1900;
      tm.tm_mon = month - 1;
      tm.tm_mday = day;
      tm.tm_hour = hour;
      tm.tm_min = minute;
      tm.tm_sec = second;
      time = mktime(&tm);

    }

    //!
    //! \brief STime konstruktor
    //! \param t czas zgodny z std
    STime (const std::time_t& t) : STime(t, localtime(&t))
    {

    }

    //!
    //! \brief getTime zwraca czas
    //! \return czas z STD
    inline long getTime() const
    {
        return time;
    }

    //!
    //! \brief getYear zwraca lata
    //! \return lata
    inline int getYear() const
    {
        return year;
    }

    //!
    //! \brief getMonth zwraca miesiace
    //! \return miesiace
    inline int getMonth() const
    {
        return month;
    }

    //!
    //! \brief getDay zwraca dni
    //! \return dni
    inline int getDay() const
    {
        return day;
    }

    //!
    //! \brief getHour zwraca godzinny
    //! \return godziny
    inline int getHour() const
    {
        return hour;
    }

    //!
    //! \brief getMinute zwraca minuty
    //! \return minuty
    inline int getMinute() const
    {
        return minute;
    }

    //!
    //! \brief getSecond zwraca sekundy
    //! \return sekundy
    inline int getSecond() const
    {
        return second;
    }

    //!
    //! \brief operator - odejmuje czasy
    //! \param time1 czas do odjecia
    //! \return roznica czasow
    STime operator-(const STime& time1)
    {
      auto timeBuf = time - time1.time;
      return convertDuration(timeBuf);
    }

    //!
    //! \brief operator + dodaje czas
    //! \param time1 czas do doawnia
    //! \return suma czasu
    STime operator+(const STime& time1)
    {
      auto timeBuf = time - time1.time;
      return convertDuration(timeBuf);
    }

    //!
    //! \brief operator < porownuje czasy
    //! \param time1 prawa strona
    //! \return true jesli lewa strona jest wczesniejsza
    bool operator<(const STime& time1)
    {
      return time < time1.time;
    }

    //!
    //! \brief operator <= porownuje czasy
    //! \param time1 prawa strona
    //! \return true jesli lewa strona jest wczesniejsza badz rowna
    bool operator<=(const STime& time1)
    {
      return time <= time1.time;
    }

    //!
    //! \brief operator > porownuje czasy
    //! \param time1 prawa strona
    //! \return true jesli lewa strona jest pozniejsza
    bool operator>(const STime& time1)
    {
      return time > time1.time;
    }

    //!
    //! \brief operator >= porownuje czasy
    //! \param time1 prawa strona
    //! \return true jesli lewa strona jest pozniejsza badz rowna
    bool operator>=(const STime& time1)
    {
      return time >= time1.time;
    }

    //!
    //! \brief display wyswielta czas
    void display()
    {
      std::cerr<<day<<"."<<month<<"."<<year<<":"<<time<<std::endl;
    }

  private:
    //!
    //! \brief convertDuration konwertuje sekundy na dni, godziny, minuty, sekundy - nie uwzglednia miesiacy
    //! \param time sekudny
    //! \return struktura czasu
    STime convertDuration(const long time)
    {
      auto timeBuf2 = time;

      int second = timeBuf2 % 60;
      timeBuf2 /= 60;
      int minute = timeBuf2 % 60;
      timeBuf2 /= 60;
      int hour = timeBuf2 % 24;
      timeBuf2 /= 24;
      int day = timeBuf2;
      return STime(time, 0, 0, day, hour, minute, second);
    }
  };

  //!
  //! \brief The CTime class klasa ze statycznymi metodami obslugi czasu.
  //!        Po zdefiniwaniu DEBUG_TIME umozliwia manipulowanie czasem
  //! \author Marcin Serwach
  class CTime{

  public:
    //!
    //! \brief The ETimeUnit enum jednostki czasu
    enum class ETimeUnit
    {
      SECOND, MINUTE, HOUR, DAY, MONTH, YEAR
    };

  private:
    //!
    //! \brief moveTime przestawia jednostke czasu
    //! \param valueToChange czas
    //! \param future true - przyszlosc, false przeszlosc
    //! \param typeValue wartosc typu czasu w sekundach tzn 1 dla sekundy, 60 dla minuty
    //! \param value zmiana w jednostkach wlasnych tzn minutach, godzinach (funkcja konwertuje na sekundy)
    inline static void moveUnitTime(long& valueToChange, const bool future, const int typeValue, const int value)
    {
      if (future)
      {
        valueToChange += value * typeValue;
      }
      else
      {
        valueToChange -= value * typeValue;
      }
    }

    //!
    //! \brief moveTime przestawia czas
    //! \param time jednostka czasu, ktora bedzie zmieniana
    //! \param future true - przyszlosc; false - przeszlosc
    //! \param unit jednostka czasu o jaka czas jest przestawiany
    //! \param value zmiana w jednostkach czasu unit
    static void moveTime(long& time, const bool future, const ETimeUnit unit, const int value)
    {
      switch(unit)
      {
      case ETimeUnit::SECOND:
        moveUnitTime(time, future, 1, value);
        break;
      case ETimeUnit::MINUTE:
        moveUnitTime(time, future, 60, value);
        break;
      case ETimeUnit::HOUR:
        moveUnitTime(time, future, 60 * 60, value);
        break;
      case ETimeUnit::DAY:
        moveUnitTime(time, future, 24 * 60 * 60, value);
        break;
      case ETimeUnit::MONTH:
        // zakladam ze kazdy miesiac ma 30 dni
        moveUnitTime(time, future, 30 * 24 * 60 * 60, value);
        break;
      case ETimeUnit::YEAR:
        // zakladam ze kazdy rok ma 365, a kazdy miesiac 30 dni
        moveUnitTime(time, future, 365 * 24 * 60 * 60, value);
        break;
      }
    }

#ifdef TEST_ENABLE
  private:
    //! \brief zmienna, dzieki ktorej czas jest przestawiany
    static long secondsMoved;

  public:

    //!
    //! \brief changeTime zmiania czas, ktory bedzie zwracany przez funkcje now
    //! \param future true przyszlosc, false przeslosc
    //! \param unit jednostka czasu
    //! \param value zmiana czasu
    //! \return aktualny, po zmianie czas
    static STime changeTime(const bool future, const ETimeUnit unit, const int value)
    {
      moveTime(secondsMoved, future, unit, value);
      return now();
    }

#endif

  public:

    //!
    //! \brief now zwraca aktualny czas
    //! \return aktualny czas
    static STime now()
    {
      auto now = std::chrono::system_clock::now();
      auto in_time_t = std::chrono::system_clock::to_time_t(now);
#ifdef TEST_ENABLE
      in_time_t += secondsMoved;
#endif
      struct tm* aTime = localtime(&in_time_t);

      return STime(in_time_t, aTime);
    }

    //!
    //! \brief getDateTime zwraca czas z uwzglednieniem zmian przy pomocy funkcji moveTime dla zdefinowanego DEBUG_TIME
    //! \param future true przyszlosc, false przeslosc
    //! \param unit jednostka czasu
    //! \param value zmiana czasu
    //! \return czas aktualny przesuniety o podane wartosci
    static STime getDateTime(const bool future, const ETimeUnit unit, const int value)
    {
      auto now = std::chrono::system_clock::now();
      auto in_time_t = std::chrono::system_clock::to_time_t(now);
#ifdef TEST_ENABLE
      in_time_t += secondsMoved;
#endif
      moveTime(in_time_t, future, unit, value);
      struct tm* aTime = localtime(&in_time_t);

      return STime(in_time_t, aTime);
    }

    static STime getChangedTime(const STime& time, const bool future, const ETimeUnit unit, const int value)
    {
      long timeL = time.getTime();
      moveTime(timeL, future, unit, value);
      return STime(timeL);
    }
  };
}

#endif // CTIME_H
