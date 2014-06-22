/*
 * SensorDataFileManager.h
 *
 *  Created on: 11 cze 2014
 *      Author: iblis
 */

#ifndef SENSORDATAFILEMANAGER_H_
#define SENSORDATAFILEMANAGER_H_
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <QDir>
#include "configuration/interfaces/ConfigurationFactory.h"
#include "util/Time.h"
#include "util/Logger.h"

#define DEBUG_SENSOR_DATA


namespace NEngine
{
  using namespace NUtil;
  const char kPathSeparator =
  #ifdef _WIN32
                              '\\';
  #else
                              '/';
  #endif
  //! \brief The CSensorDataFileManager class jest odpowiedzialna za zapis/odczyt/aktualizacje pliku z danymi z czujnikow
  //!         T - klasa czujnikow
  //!         SeriesAround - ilosc pomiaro przed i po pomiarach niebezpiecznych, ktore maja byc zapamietane.
  //!
  //! 6 plikow kazdego dnia. Prefix nazwy pliku jest data.
  //!
  //! I. buffer
  //! 1) .buffer - plik do ktorego sa cyklicznie zapisywane dane, zawiera info i seriesAround serii
  //! struktura danych:
  //! | SBufferInfo | id_A | data_A1 | data_A2 |... | data_An | id_B | data_B1 | data_B2 | ... | data_Bn | .. | id_X | ..
  //! 2) ._confirmed.buffer indeksy potwierdzony z serwera serii
  //! struktura danych:
  //! | rozmiar | id_A | id_B | ...
  //! 3) ._unconfirmed.buffer indeksy czekajace na potwierdzenie
  //!  struktura danych:
  //! | rozmiar | id_A | id_B | ...
  //!
  //! II. warning
  //! 1) .warnings - plik z danymi
  //! struktura danych:
  //! | SWarningsInfo | id_A | data_A1 | data_A2 |... | data_An | id_B | data_B1 | data_B2 | ... | data_Bn | .. | id_X | ..
  //! 2) ._confirmed.warnings indeksy potwierdzony z serwera serii wraz z offsetami w .warnings
  //!  struktura danych:
  //!  | rozmiar | id_A | offset id_A w .warings | id_B | offset id_B w .warings |  ...
  //! 3) ._unconfirmed.warnings indeksy czekajace na potwierdzenie wraz z offsetami w .warnings
  //!  struktura danych:
  //!  | rozmiar | id_A | offset id_A w .warings | id_B | offset id_B w .warings |  ...
  //! W pliku .buffer nigdy nie znajduja sie dane niebezpieczne - sa odrazu do .warning zapisywane wraz z aktualnym stanem .buffer
  //!
  //! Ponadto jeden plik savedSeriesDate.list zawierajacy daty, dla ktorych powyzej wymienione pliki istnieja. Jesli nie bedzie miejsca na karcie
  //! to zostana usuniete pliki najstarsze a plik savedSerieDate.list zaktualizowany.
  //! Struktura pliku:
  //! | size | data1 | data2 | ....
  //!
  template<typename T, int SeriesAround>
  class CSensorDataFileManager
  {


    //! \brief Pierwsze bajty pliku .buffer z informacjami
    struct SBufferInfo
    {
      static const int noWarning = -1;
      int size;   //!< ilosc serii w pliku
      int warning;  //!< ilosc serii od warninga; -1 (SBufferInfo::noWarning), czyli nie bylo warninga
      int sensors;  //!< ilosc czujnikow
    };

    //! \brief Pierwsze bajty pliku .warnings z informacjami
    struct SWarningsInfo
    {
      int size;     //!< ilosc serii w pliku
      int sensors;  //!<  ilosc czujnikow
    };

    //! \brief Indeksy w plikach ._confirmed.warnings i ._unconfirmed.warnings
    struct SWarningIndex
    {
      int id;     //!< id serii
      int offset; //!< offset w .warnings
    };

    //! \brief folder z plikami
    inline static std::string getFolder()
    {
      const static std::string folder("data");
      return folder;
    }

    //! \brief zwraca sciezke do folderu danych
    //! \return sciezka do folderu z danymi
    inline static std::string getFolderPath()
    {
      std::string dataPath = CConfigurationFactory::getInstance()->getDataPath();
      return dataPath + kPathSeparator + getFolder() + kPathSeparator;
    }

    //! \brief getDateString zwraca string otrzymany z daty - string ten jest wykorzystywany jako czesc nazwy plikow
    //! \param[in] daysBeforeNow ilosc dni przed dzisiaj
    //! \return string z data
    static std::string getDateString(const int daysBeforeNow)
    {
      NUtil::STime time = NUtil::CTime::getDateTime(false, NUtil::CTime::ETimeUnit::DAY, daysBeforeNow);
      return std::to_string(time.getDay()) + "_" + std::to_string(time.getMonth()) + "_" + std::to_string(time.getYear());
    }

    //!
    //! \brief getFilePath funkcja zwraca sciezke z prefixem do pliku. Prefix jest data
    //! \param[in] daysBeforeNow ilosc dni przed dzisiaj
    //! \return sciezka z prefixem pliku
    inline static std::string getFilePath(const int daysBeforeNow)
    {
      return getFolderPath() + getDateString(daysBeforeNow);
    }

    //! \brief getWarningFilePath zwraca pelna sciezke do pliku .warnings. Sciezka zalezna od daty.
    //! \param[in] daysBeforeNow ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getWarningFilePath(const int daysBeforeNow)
    {
      static const char* buf = ".warnings";
      return getFilePath(daysBeforeNow) + buf;
    }

    //! \brief getWarningFilePath zwraca pelna sciezke do pliku .warnings.
    //! \param[in] folderPath sciezka do folderu
    //! \param[in] filePathWithoutExt ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getWarningFilePath(const std::string& folderPath, const std::string& filePathWithoutExt)
    {
      static const char* buf = ".warnings";
      return folderPath + filePathWithoutExt + buf;
    }

    //! \brief getConfirmedWarningFilePath zwraca pelna sciezke do pliku ._confirmed.warnings. Sciezka zalezna od daty.
    //! \param[in] daysBeforeNow ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getConfirmedWarningFilePath(
        const int daysBeforeNow)
    {
      static const char* buf = "_confirmed.warnings";
      return getFilePath(daysBeforeNow) + buf;
    }

    //! \brief getConfirmedWarningFilePath zwraca pelna sciezke do pliku ._confirmed.warnings.
    //! \param[in] folderPath sciezka do folderu
    //! \param[in] filePathWithoutExt ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getConfirmedWarningFilePath(const std::string& folderPath, const std::string& filePathWithoutExt)
    {
      static const char* buf = "_confirmed.warnings";
      return folderPath + filePathWithoutExt + buf;
    }

    //! \brief getUnconfirmedWarningFilePath zwraca pelna sciezke do pliku ._unconfirmed.warnings. Sciezka zalezna od daty.
    //! \param[in] daysBeforeNow ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getUnconfirmedWarningFilePath(
        const int daysBeforeNow)
    {
      static const char* buf = "_unconfirmed.warnings";
      return getFilePath(daysBeforeNow) + buf;
    }

    //! \brief getUnconfirmedWarningFilePath zwraca pelna sciezke do pliku ._unconfirmed.warnings.
    //! \param[in] folderPath sciezka do folderu
    //! \param[in] filePathWithoutExt ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getUnconfirmedWarningFilePath(const std::string& folderPath, const std::string& filePathWithoutExt)
    {
      static const char* buf = "_unconfirmed.warnings";
      return folderPath + filePathWithoutExt + buf;
    }

    //! \brief getBufferFilePath zwraca pelna sciezke do pliku .buffer. Sciezka zalezna od daty.
    //! \param[in] daysBeforeNow ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getBufferFilePath(const int daysBeforeNow)
    {
      static const char* buf = ".buffer";
      return getFilePath(daysBeforeNow) + buf;
    }

    //! \brief getBufferFilePath zwraca pelna sciezke do pliku .buffer.
    //! \param[in] folderPath sciezka do folderu
    //! \param[in] filePathWithoutExt ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getBufferFilePath(const std::string& folderPath, const std::string& filePathWithoutExt)
    {
      static const char* buf = ".buffer";
      return folderPath + filePathWithoutExt + buf;
    }

    //! \brief getConfirmedBufferFilePath zwraca pelna sciezke do pliku _confirmed.buffer. Sciezka zalezna od daty.
    //! \param[in] daysBeforeNow ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getConfirmedBufferFilePath(
        const int daysBeforeNow)
    {
      static const char* buf = "_confirmed.buffer";
      return getFilePath(daysBeforeNow) + buf;
    }

    //! \brief getConfirmedBufferFilePath zwraca pelna sciezke do pliku _confirmed.buffer.
    //! \param[in] folderPath sciezka do folderu
    //! \param[in] filePathWithoutExt ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getConfirmedBufferFilePath(const std::string& folderPath, const std::string& filePathWithoutExt)
    {
      static const char* buf = "_confirmed.buffer";
      return folderPath + filePathWithoutExt + buf;
    }

    //! \brief getUnconfirmedBufferFilePath zwraca pelna sciezke do pliku _unconfirmed.buffer. Sciezka zalezna od daty.
    //! \param[in] daysBeforeNow ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getUnconfirmedBufferFilePath(
        const int daysBeforeNow)
    {
      static const char* buf = "_unconfirmed.buffer";
      return getFilePath(daysBeforeNow) + buf;
    }

    //! \brief getUnconfirmedBufferFilePath zwraca pelna sciezke do pliku _unconfirmed.buffer.
    //! \param[in] folderPath sciezka do folderu
    //! \param[in] filePathWithoutExt ile dni od dzisiaj
    //! \return pelna sciezka
    inline static std::string getUnconfirmedBufferFilePath(const std::string& folderPath, const std::string& filePathWithoutExt)
    {
      static const char* buf = "_unconfirmed.buffer";
      return folderPath + filePathWithoutExt + buf;
    }

    //! \brief getFileListFilePath zwarca sciezke do savedSeriesDate.list
    //! \return sciezka do savedSeriesDate.list
    inline static std::string getDateListFilePath()
    {
      DConfiguration configuration = CConfigurationFactory::getInstance();
      const std::string dataPath = configuration->getDataPath();
      static const char* fileName = "savedSeriesDate.list";
      return dataPath + kPathSeparator + getFolder() + kPathSeparator + fileName;
    }

    //! \brief dodaje date do pliku z lista wszystkich dat
    //! \param[in] fileName
    //! \return true jesli wszystko ok
    static bool addDateToDateListFile()
    {
      const STime time = CTime::now();
      std::fstream file(getDateListFilePath(), std::fstream::in |std::fstream::out | std::fstream::binary | std::fstream::ate);
      if (file.is_open())
      {
        // plik istnieje wiec zmien size i dodaj date
        changeSizeInFile(file, 1, true);
        file.seekg(0, std::fstream::end);
        long buf = time.getTime();
        file.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
        file.flush();
        file.close();
        return true;
      }
      else
      {
        // plik nie istnieje wiec stworz nowy
        std::string fileStr = getDateListFilePath();

        std::ofstream newFile(getDateListFilePath(), std::fstream::out | std::fstream::binary);
        if (newFile.is_open())
        {
          static const int defaultSize = 1;
          newFile.write(reinterpret_cast<const char*>(&defaultSize), sizeof(defaultSize));
          long buf = time.getTime();
          file.write(reinterpret_cast<const char*>(&buf), sizeof(buf));
          newFile.flush();
          newFile.close();
          return true;
        }
        else
        {
          LOG_ERROR("Adding date to file list failed");
          return false;
        }
      }
    }

    //! \brief Usuwa stare pliki. Jesli w pliku sa dane z 2 miesiecy, to dane ze starszego miesiaca sa usuwane
    //! \return true jesli wszystko ok, false jesli nie
    static bool cleanOldFiles()
    {
      const std::string dateListFilePath = getDateListFilePath();
      std::fstream file(dateListFilePath, std::fstream::in |std::fstream::out | std::fstream::binary | std::fstream::ate);
      if (file.is_open())
      {
        // przeczytaj rozmiar
        int size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        // dni w miesiacu
        static const int monthDays = 30;
        if (size < 2 * monthDays)
        {
          file.close();
          return true;
        }
        else
        {
          std::vector<std::time_t> dates;
          readDateListFile(dates);
          file.close();

          // stala statyczna tablica wskaznikow do funkcji :)
          typedef std::string (*const  DFunPointerArray[])(const int);
          static const DFunPointerArray funArray = {getBufferFilePath,
                                                    getConfirmedBufferFilePath,
                                                    getUnconfirmedBufferFilePath,
                                                    getWarningFilePath,
                                                    getConfirmedWarningFilePath,
                                                    getUnconfirmedWarningFilePath};

          STime now = CTime::now();
          for (int i = 0; i < monthDays; ++i)
          {
            // iteracja po tablicy wskaznikow do funkcji
            for (auto& j : funArray)
            {
              STime diff = now - STime(dates[i]);
              std::string filePath = j(diff.getDay());
              remove(filePath.c_str());
            }
          }
          // usuniecie pliku
          remove(dateListFilePath.c_str());

          // zapisz nowe dane - stworz plik
          std::ofstream newFile(dateListFilePath, std::fstream::out | std::fstream::binary);
          if (newFile.is_open())
          {
            size -= monthDays;
            newFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
            newFile.write(reinterpret_cast<const char*>(dates.data() + monthDays), sizeof(std::time_t) * size);
            newFile.flush();
            newFile.close();
            return true;
          }
          else
          {
            // nie mozna bylo otworzyc nowo utworzonego pliku
            return false;
          }
        }
      }
      else
      {
        // nie mozna bylo otworzyc pliku
        return false;
      }
    }

    //!
    //! \brief saveNewFile zapisuje nowe pliki
    //! \param[in] yesterdayWarning ile dni od dzisiaj (nazwy plikow zalezne od daty)
    //! \param[in] actualWarning czy dopisywana bedzie seria niebezpieczna
    //! \param[in] sData pomiary
    //! \return ID serii
    static int saveNewFile(const bool yesterdayWarning,
        const bool actualWarning, const std::vector<T>& sData)
    {
      // id
      const int id = 0;

      const int sensors = sData.size();
      // utworz plik buforow
      // | info | dane | dane |
      const std::string bufferFilePath = getBufferFilePath(0);
      std::ofstream bufferFile(bufferFilePath,
          std::ofstream::out | std::ofstream::binary);
      // wypelnianie pliku
      // info; ustaw flage jesli ostatnia seria byla niebezpieczna lub obecna;
      // obecna niebezpieczna jest od razu zapisywana do .warnings
      SBufferInfo bufferInfo;
      if (yesterdayWarning || actualWarning)
      {
        bufferInfo.sensors = sensors;
        // zapis od razu do .warnings, wiec jest 0 serii w .buffer
        bufferInfo.size = 0;
        // byl ostatnio warning
        bufferInfo.warning = 0;
      }
      else
      {
        bufferInfo.sensors = sensors;
        bufferInfo.size = 1;
        bufferInfo.warning = SBufferInfo::noWarning;
      }

      bufferFile.write(reinterpret_cast<const char*>(&bufferInfo),
          sizeof(bufferInfo));
      if (!actualWarning)
      {
        // jesli aktualny pomiar nie jest niebezpieczny to od razu zapisz do bufora
        bufferFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
        // wypelnienie danymi
        bufferFile.write(reinterpret_cast<const char*>(sData.data()),
            sensors * sizeof(T));
      }
      bufferFile.flush();
      bufferFile.close();

      // utworzenie pliku bufora z indeksami niepotwierdzonymi
      // | size | flaga | flaga |
      const std::string bufferUnconfirmedFilePath =
          getUnconfirmedBufferFilePath(0);
      std::ofstream bufferUnconfirmedFile(bufferUnconfirmedFilePath,
          std::ofstream::out | std::ofstream::binary);
      if (!actualWarning)
      {
        const int size = 1;
        int data[] =
        { size, id };
        bufferUnconfirmedFile.write(reinterpret_cast<const char*>(&data),
            sizeof(data));
      }
      else
      {
        const int size = 0;
        bufferUnconfirmedFile.write(reinterpret_cast<const char*>(&size),
            sizeof(size));
      }
      bufferUnconfirmedFile.flush();
      bufferUnconfirmedFile.close();

      // utworzenie pliku bufora z indeksami potwierdzonymi - pusty plik
      // | size | flaga | flaga |
      const std::string bufferConfirmedFilePath = getConfirmedBufferFilePath(0);
      std::ofstream bufferConfirmedFile(bufferConfirmedFilePath,
          std::ofstream::out | std::ofstream::binary);
      const int sizeConfirmed = 0;
      bufferConfirmedFile.write(reinterpret_cast<const char*>(&sizeConfirmed),
          sizeof(sizeConfirmed));
      bufferConfirmedFile.flush();
      bufferConfirmedFile.close();

      // utworzenie pliku z ostrzezeniami
      // | size | id | dane | id | dane .. |
      const std::string warningFilePath = getWarningFilePath(0);
      std::ofstream warningFile(warningFilePath,
          std::ofstream::out | std::ofstream::binary);
      if (actualWarning)
      {
        // jesli aktualny pomiar jest niebezpieczny to od razu zapisz do danych niebezpiecznych

        const int size = 1;
        SWarningsInfo warningInfo
        { size, sensors };
        warningFile.write(reinterpret_cast<const char*>(&warningInfo),
            sizeof(warningInfo));
        warningFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
        // wypelnienie danymi
        warningFile.write(reinterpret_cast<const char*>(sData.data()),
            sensors * sizeof(T));
      }
      else
      {
        const int size = 0;
        SWarningsInfo warningInfo
        { size, sensors };
        warningFile.write(reinterpret_cast<const char*>(&warningInfo),
            sizeof(warningInfo));
      }
      warningFile.flush();
      warningFile.close();

      // utworzenie pliku bufora z ostrzezeniami niepotwierdzonymi
      // | size | id | offset w .warnings | id | offset w .warnings
      const std::string warningUnconfirmedFilePath =
          getUnconfirmedWarningFilePath(0);
      std::ofstream warningUnconfirmedFile(warningUnconfirmedFilePath,
          std::ofstream::out | std::ofstream::binary);
      if (actualWarning)
      {
        // rozmiar
        const int size = 1;
        warningUnconfirmedFile.write(reinterpret_cast<const char*>(size),
            sizeof(size));
        //  offset do nowego
        const int offset = 0;
        SWarningIndex warningIndex
        { id, offset };
        warningUnconfirmedFile.write(
            reinterpret_cast<const char*>(&warningIndex), sizeof(warningIndex));
      }
      else
      {
        const int size = 0;
        warningUnconfirmedFile.write(reinterpret_cast<const char*>(&size),
            sizeof(size));
      }
      warningUnconfirmedFile.flush();
      warningUnconfirmedFile.close();

      // utworzenie pliku bufora z ostrzezeniami potwierdzonymi - pusty plik
      // id | offset w .warnings | id | offset w .warnings
      const std::string warningConfirmedFilePath = getConfirmedWarningFilePath(
          0);
      std::ofstream warningConfirmedFile(warningConfirmedFilePath,
          std::ofstream::out | std::ofstream::binary);
      const int sizeConfirmed2 = 0;
      warningConfirmedFile.write(reinterpret_cast<const char*>(&sizeConfirmed2),
          sizeof(sizeConfirmed2));
      warningConfirmedFile.flush();
      warningConfirmedFile.close();

      // dodaj informacje o nowym pliku do pliku list
      addDateToDateListFile();
      return id;
    }

    //!
    //! \brief readDateListFile Odczytuje
    //! \param file
    //! \param filePaths
    static void readDateListFile(std::fstream& file, std::vector<std::time_t>& filePaths)
    {
      // przeczytaj rozmiar
      int size;
      file.read(reinterpret_cast<char*>(&size), sizeof(size));

      // przeczytaj dane wszystkie
      std::shared_ptr<std::time_t> array = getArray<std::time_t>(size);
      std::time_t* buf = &*array;
      file.read(reinterpret_cast<char*>(buf), size * sizeof(std::time_t));

      std::for_each(buf, buf + size, [&](std::time_t& t){filePaths.push_back(t);});
    }


    static bool readDateListFile(std::vector<std::time_t>& filePaths)
    {

      std::fstream file(getDateListFilePath(), std::fstream::in |std::fstream::out | std::fstream::binary | std::fstream::ate);
      if (file.is_open())
      {
        readDateListFile(file, filePaths);
        file.close();
        return true;
      }
      return false;
    }
    //!
    //! \brief wasWarning sprawdza czy ostatnie serie danego dnia byla niebezpiecznia
    //! \param[in] daysBeforeNow ile dni od dzisiaj
    //! \return true jesli serie byly niezbezpieczne
    static bool wasWarning()
    {
      STime now = CTime::now();
      std::vector<std::time_t> fileList;

      SBufferInfo bufferInfo;

      // odczytaj liste dat zapisanych
      if (readDateListFile(fileList))
      {
        // przechodz od najstarszej
          STime dif = now - STime(*fileList.rbegin());
          std::fstream bufferFile(getBufferFilePath(dif.getDay()),
              std::fstream::in | std::fstream::binary);
          bufferFile.read(reinterpret_cast<char*>(&bufferInfo), sizeof(bufferInfo));
          bufferFile.close();
          return (bufferInfo.warning != SBufferInfo::noWarning) && (bufferInfo.warning < SeriesAround);
      }
      return false;
    }

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

    //!
    //! \brief readIndexFile wczytuje pliki indeksow
    //! \param[in,out] file plik
    //! \param[out] data odczytane id
    //! \param[in] clean czy czyscic plik - ustawic rozmiar na 0
    template<typename T2>
    static void readIndexFile(std::fstream& file, std::vector<T2>& data,
        const bool clean)
    {

      file.seekg(0, std::fstream::beg);
      int size;
      file.read(reinterpret_cast<char*>(&size), sizeof(size));

      if (size > 0)
      {
        data.reserve(size);
        std::shared_ptr<T2> p = getArray<T2>(size);
        // odczytaj dane
        file.read(reinterpret_cast<char*>(&*p), sizeof(T2) * size);
        // skopiuj do vectora
        data.insert(data.end(), &*p, &*p + size);
        if (clean)
        {
          size = 0;
          file.seekg(0, std::fstream::beg);
          file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        }

      }
    }

    //!
    //! \brief writeIndexFile zapisuje/dopisuje dane do plikow indeksow
    //! \param[in] file plik
    //! \param[in] data wektor danych
    //! \param[in] append true jesli dopisac
    template<typename T2>
    static void writeIndexFile(std::fstream& file, const std::vector<T2>& data,
        const bool append)
    {
      file.seekg(0, std::fstream::beg);
      int size;
      int oldSize;
      if (append)
      {
        file.read(reinterpret_cast<char*>(&oldSize), sizeof(oldSize));
        size = oldSize + data.size();
        file.seekg(0, std::fstream::beg);

      }
      else
      {
        size = data.size();
        oldSize = 0;
      }
      file.write(reinterpret_cast<const char*>(&size), sizeof(size));
      file.seekg(sizeof(size) + oldSize * sizeof(T2), std::fstream::beg);

      file.write(reinterpret_cast<const char*>(data.data()),
          sizeof(T2) * data.size());
    }

    //!
    //! \brief saveDataWarning zapisuje dane niebezpieczne
    //! \param[in,out] bufferFile plik .buffer
    //! \param[in] sData seria danych
    //! \return id serii
    static int saveDataWarning(std::fstream& bufferFile,
        const std::vector<T>& sData)
    {
      std::fstream warningFile(getWarningFilePath(0),
          std::fstream::in | std::fstream::out | std::fstream::binary);
      std::fstream warningConfirmedFile(getConfirmedWarningFilePath(0),
          std::fstream::in | std::fstream::out | std::fstream::binary);
      std::fstream warningUnconfirmedFile(getUnconfirmedWarningFilePath(0),
          std::fstream::in | std::fstream::out | std::fstream::binary);
      std::fstream bufferConfirmedFile(getConfirmedBufferFilePath(0),
          std::fstream::in | std::fstream::out | std::fstream::binary);
      std::fstream bufferUnconfirmedFile(getUnconfirmedBufferFilePath(0),
          std::fstream::in | std::fstream::out | std::fstream::binary);

      // odczytaj info buffera
      SBufferInfo bufferInfo;
      bufferFile.read(reinterpret_cast<char*>(&bufferInfo), sizeof(bufferInfo));

      // skopiuj zwartosc .buffer do .warnings
      const int lastId = moveDataToWarnings(bufferInfo, warningFile,
          warningConfirmedFile, warningUnconfirmedFile, bufferFile,
          bufferConfirmedFile, bufferUnconfirmedFile, true);

      warningFile.seekg(0, std::fstream::end);
      warningUnconfirmedFile.seekg(0, std::fstream::end);
      // id nowego
      const int id = lastId + 1;
      // dopisz wpis do _unconfirmed.warnings
      SWarningIndex index
      { id, warningFile.tellg() };
      warningUnconfirmedFile.write(reinterpret_cast<const char*>(&index),
          sizeof(index));
      // dopisz aktualne dane do .warnings - id, dane
      warningFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
      warningFile.write(reinterpret_cast<const char*>(sData.data()),
          sizeof(T) * sData.size());
      changeSizeInFile(warningFile, 1, true);
      // zamykanie wszystkiego
      warningFile.flush();
      warningFile.close();
      warningUnconfirmedFile.flush();
      warningUnconfirmedFile.close();

      warningConfirmedFile.close();
      bufferFile.close();
      bufferConfirmedFile.close();
      bufferUnconfirmedFile.close();

      return id;
    }

    //!
    //! \brief moveDataToWarnings przenosi dane z buffer do warning
    //! \param bufferInfo info z .buffer
    //! \param warningFile plik .warning
    //! \param warningConfirmedFile plik _confirmed.warning
    //! \param warningUnconfirmedFile plik _unconfirmed.warning
    //! \param bufferFile plik .buffer
    //! \param bufferConfirmedFile plik _confirmed.buffer
    //! \param bufferUnconfirmedFile plik _unconfirmed.buffer
    //! \param warning true gdy zapisywany bedzie pomiar niebezpieczny
    //! \return id ostatniej serii
    static int moveDataToWarnings(SBufferInfo& bufferInfo,
        std::fstream& warningFile, std::fstream& warningConfirmedFile,
        std::fstream& warningUnconfirmedFile, std::fstream& bufferFile,
        std::fstream& bufferConfirmedFile, std::fstream& bufferUnconfirmedFile,
        const bool warning)
    {
      // kopiujemy z pliku .buffer do pliku .warnings (oczywiscie bez info)
      // ustawienie pozycji
      bufferFile.seekg(sizeof(bufferInfo), std::fstream::beg);
      // odczytane dane: ilosc serii * (ilosc czujnikow * rozmiar danych + rozmiar id)
      const int allBufferSeriesSize = bufferInfo.size
          * (bufferInfo.sensors * sizeof(T) + sizeof(int));

      std::shared_ptr<char> array = getArray<char>(allBufferSeriesSize);

      bufferFile.read(reinterpret_cast<char*>(&*array), allBufferSeriesSize);

      // przeczytaj pliki bufora niepotwierdzonego
      std::vector<int> unconfirmed;
      readIndexFile(bufferUnconfirmedFile, unconfirmed, warning);

      // przeczytaj pliki bufora potwierdzonego
      std::vector<int> confirmed;
      readIndexFile(bufferConfirmedFile, confirmed, warning);

      // rzygotowanie pliku .warnings do zapisu - przejscie na koniec pliku
      warningFile.seekg(0, std::fstream::end);

      const int seriesSize = sizeof(int) + bufferInfo.sensors * sizeof(T);
      int* idPointer = nullptr;

      std::vector<SWarningIndex> newUnconfirmed;
      std::vector<SWarningIndex> newConfirmed;
      int idOffsetWarnings = warningFile.tellg();
      for (int i = 0; i < bufferInfo.size; ++i)
      {
        idPointer = reinterpret_cast<int*>(&*array + i * seriesSize);
        // utworz strukture wpisu (id, offset w .warnings)
        SWarningIndex index
        { *idPointer, idOffsetWarnings + i * seriesSize };

        // czy seria byla w potwierdzonych
        if (std::find(confirmed.begin(), confirmed.end(), *idPointer)
            != confirmed.end())
        {
          // byl w potwierdzonych wiec dodaje index do _confirmed.warnings
          newConfirmed.push_back(index);
        }
        else
        {
          // seria nie byla w potwierdzonych, ale byla w niepotwierdzonych
          if (std::find(unconfirmed.begin(), unconfirmed.end(), *idPointer)
              != unconfirmed.end())
          {
            newUnconfirmed.push_back(index);
          }
          else
          {
            LOG_ERROR("Move from .buffer to .warnings failed");
          }
        }

      }

      // zapisz _unconfirmed.warnings
      writeIndexFile(warningUnconfirmedFile, newUnconfirmed, true);

      // zapisz _confirmed.warnings
      writeIndexFile(warningConfirmedFile, newConfirmed, true);

      // uaktualnij ilosc w pliku .warnings
      changeSizeInFile(warningFile, bufferInfo.size, true);
      // zapisz dane do .warnings
      warningFile.seekg(0, std::fstream::end);
      warningFile.write(&*array, allBufferSeriesSize);

      // uaktualnij ._confirmed.warnings
      changeSizeInFile(warningConfirmedFile, confirmed.size(), true);

      // uaktualnij ._unconfirmed.warnings
      changeSizeInFile(warningUnconfirmedFile, unconfirmed.size(), true);

      if (warning)
      {
        // jesli warning bedzie dodawany, to odrazu uzupelnil dane
        // ustaw rozmiar na 0 - bo .buffer bedzie pusty
        bufferInfo.size = 0;
        // flage warningu
        bufferInfo.warning = 0;
      }
      else
      {
        bufferInfo.size = 1;
        // flage warningu na SBufferInfo::noWarning, czyli nie bylo warningu
        bufferInfo.warning = SBufferInfo::noWarning;
      }

      bufferFile.seekg(0, std::fstream::beg);
      bufferFile.write(reinterpret_cast<const char*>(&bufferInfo),
          sizeof(bufferInfo));
      bufferFile.close();

      // zapisz bufory plikow
      warningFile.flush();
      warningConfirmedFile.flush();
      warningUnconfirmedFile.flush();
      bufferFile.flush();
      bufferConfirmedFile.flush();
      bufferUnconfirmedFile.flush();

      // zwraca id ostatniego pomiaru
      return *idPointer;
    }

    //!
    //! \brief changeSizeInFile zmienia rozmiar danych w pliku (rozmiar to pierwszy int w pliku)
    //! \param file plik do zmiany
    //! \param size rozmian do zmiany
    //! \param add true jesli zwiekszych rozmian o size, false jesli nadpisac
    static void changeSizeInFile(std::fstream& file, const int size,
        const bool add)
    {
      int newSize;
      file.seekg(0, std::fstream::beg);
      if (add)
      {
        // jesli dodac do rozmiaru to odczytaj stary
        file.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));
        newSize += size;
        file.seekg(0, std::fstream::beg);
      }
      else
      {
        // nadpisac rozmiar
        newSize = size;
      }

      file.write(reinterpret_cast<const char*>(&newSize), sizeof(newSize));
    }

    //!
    //! \brief saveDataNotWarning zapisuje dane bezpieczne
    //! \param bufferFile plik .buffer
    //! \param sData seria danych
    //! \return id
    static int saveDataNotWarning(std::fstream& bufferFile,
        const std::vector<T>& sData)
    {
      SBufferInfo bufferInfo;
      bufferFile.read(reinterpret_cast<char*>(&bufferInfo), sizeof(bufferInfo));

      std::fstream bufferUnconfirmedFile(getUnconfirmedBufferFilePath(0),
          std::fstream::in | std::fstream::out | std::fstream::binary);
      int id;
      if (bufferInfo.warning == SeriesAround)
      {
        // .buffer znajduja sie serie tuz po warningu
        // nalezy je przenies
        // najpier otworz wszystkie pliki
        std::fstream warningFile(getWarningFilePath(0),
            std::fstream::in | std::fstream::out | std::fstream::binary);
        std::fstream warningConfirmedFile(getWarningFilePath(0),
            std::fstream::in | std::fstream::out | std::fstream::binary);
        std::fstream warningUnconfirmedFile(getWarningFilePath(0),
            std::fstream::in | std::fstream::out | std::fstream::binary);
        std::fstream bufferConfirmedFile(getWarningFilePath(0),
            std::fstream::in | std::fstream::out | std::fstream::binary);
        // skopiuj zwartosc .buffer do .warnings
        const int lastId = moveDataToWarnings(bufferInfo, warningFile,
            warningConfirmedFile, warningUnconfirmedFile, bufferFile,
            bufferConfirmedFile, bufferUnconfirmedFile, true);

        // id nowego
        id = lastId + 1;
        // dane zostaly przeniesione wiec uaktualnij bufferIno
        // 1 od razu przygotuj do zapisu
        bufferInfo.size = 1;
        bufferInfo.warning = SBufferInfo::noWarning;

        warningFile.flush();
        warningFile.close();
        warningUnconfirmedFile.flush();
        warningUnconfirmedFile.close();
        warningConfirmedFile.close();
        bufferConfirmedFile.close();

        // przejdz do miejsca zapisu
        bufferFile.seekg(
            sizeof(bufferInfo)
                + (bufferInfo.size - 1) * (sizeof(int) + sizeof(T)),
            std::fstream::beg);
        bufferFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
        bufferFile.write(reinterpret_cast<const char*>(sData.data()),
            sizeof(T) * sData.size());

        // uaktualnij info
        bufferFile.seekg(0, std::fstream::beg);
        bufferFile.write(reinterpret_cast<const char*>(&bufferInfo),
            sizeof(bufferInfo));

        // uaktualnij _unconfirmed.buffer
        changeSizeInFile(bufferUnconfirmedFile, bufferInfo.size, false);
        bufferUnconfirmedFile.write(reinterpret_cast<const char*>(&id),
            sizeof(id));
      }
      else
      {

        int lastId = getLastId(bufferFile);
        id = lastId + 1;
        if (bufferInfo.size < SeriesAround)
        {
          // czyli zapis do nowego pliku lub zapis do pliku po danych tuz za warningiem
          ++bufferInfo.size;
          if (bufferInfo.warning != SBufferInfo::noWarning)
          {
            ++bufferInfo.warning;
          }
          // przejdz do miejsca zapisu
          const int savingOffset = sizeof(bufferInfo)
              + (bufferInfo.size - 1)
                  * (sizeof(int) + bufferInfo.sensors * sizeof(T));
          bufferFile.seekg(savingOffset, std::fstream::beg);

          // dopisz dane
          bufferFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
          bufferFile.write(reinterpret_cast<const char*>(sData.data()),
              sizeof(T) * bufferInfo.sensors);

          // uaktualnij info
          bufferFile.seekg(0, std::fstream::beg);
          bufferFile.write(reinterpret_cast<const char*>(&bufferInfo),
              sizeof(bufferInfo));

          // uaktualnij _unconfirmed.buffer
          const int sizeUnconfirmed = bufferInfo.size;
          changeSizeInFile(bufferUnconfirmedFile, sizeUnconfirmed, false);
          const int savingUnconfirmedOffset = sizeof(sizeUnconfirmed)
              + (bufferInfo.size - 1) * sizeof(id);
          bufferUnconfirmedFile.seekg(savingUnconfirmedOffset,
              std::fstream::beg);
          bufferUnconfirmedFile.write(reinterpret_cast<const char*>(&id),
              sizeof(id));
        }
        else
        {
          // dane trzeba cyklicznie przesunac i dodac nowe, czyli na poczatku jest tak
          // info | dane1 | dane2 | dane3 |
          // trzeba zamienic na
          // info | dane2 | dane3 | nowe dane |
          // odczytane dane: ilosc serii * (ilosc czujnikow * rozmiar danych + rozmiar id)
          const int savedBufferSeriesSize = bufferInfo.size
              * (bufferInfo.sensors * sizeof(T) + sizeof(int));
          // dodatkowe miejsca na nowa serie
          const int bufferSize = savedBufferSeriesSize + sizeof(int)
              + bufferInfo.sensors * sizeof(T);

          std::shared_ptr<char> array = getArray<char>(bufferSize);

          char* buf = &*array;
          bufferFile.seekg(sizeof(bufferInfo), std::fstream::beg);
          bufferFile.read(buf, savedBufferSeriesSize);

          // dodaj nowy pomiar
          char* newSeriesOffset = buf + savedBufferSeriesSize;

          memcpy(newSeriesOffset, &id, sizeof(id));
          memcpy(newSeriesOffset + sizeof(id), sData.data(),
              bufferInfo.sensors * sizeof(T));
          // zapisz do pliku
          // uaktualnij info
          if (bufferInfo.warning != SBufferInfo::noWarning)
          {
            ++bufferInfo.warning;
          }

          bufferFile.seekg(0, std::fstream::beg);
          bufferFile.write(reinterpret_cast<const char*>(&bufferInfo),
              sizeof(bufferInfo));
          // zapisz serie
          char* with1stSeriesOffset = buf + sizeof(int)
              + bufferInfo.sensors * sizeof(T);
          bufferFile.write(with1stSeriesOffset, savedBufferSeriesSize);

          // id nadpisywanego wpisu w plikach indexowych
          int* idOverwritten = reinterpret_cast<int*>(buf);
          // uaktualnij pliki indexowe
          // napierw _unconfirmed.buffer
          if (!removeIdFromBufferIndexFile(bufferUnconfirmedFile,
              *idOverwritten, id))
          {
            // id nie znajduje sie w  _unconfirmed.buffer
            // nadpisywana seria powinna miec index w _confirmed.buffer
            std::fstream bufferConfirmedFile(getWarningFilePath(0),
                std::fstream::in | std::fstream::out | std::fstream::binary);
            if (!removeIdFromBufferIndexFile(bufferConfirmedFile,
                *idOverwritten, id))
            {
              LOG_ERROR("id wasn't found in _unconfirmed.buffer neither _confirmed.buffer");
            }
            bufferConfirmedFile.flush();
            bufferConfirmedFile.close();
          }

        }
      }
      // dopisz dane

      bufferFile.flush();
      bufferFile.close();
      bufferUnconfirmedFile.flush();
      bufferUnconfirmedFile.close();
      return id;
    }

    //!
    //! \brief findLastUnconfirmedData Szuka ostatnich danych do potwierdzenia
    //! \param dayBeforeNow ilosc dni przed dzisiaj
    //! \param file plik _uncomfirmed.warning (otwarty jesli znaleziono)
    //! \param size rozmiar danych do wyslania
    //! \return true jesli znaleziono jakies dane
    static bool findLastUnconfirmedData(int& dayBeforeNow, std::fstream& file, int& size)
    {
      std::vector<std::time_t> filePaths;
      STime now = CTime::now();
      if (readDateListFile(filePaths))
      {
        for(auto it = filePaths.rbegin(), end = filePaths.rend(); it != end; ++it)
        {
            STime dif = now - STime(*it);
            file.open(getUnconfirmedWarningFilePath(dif.getDay()), std::fstream::out | std::fstream::in | std::fstream::binary);
            if (file.is_open())
            {
              // odczytaj ile jest danych do wyslania
              file.read(reinterpret_cast<char*>(&size), sizeof(size));
              if (size > 0)
              {
                dayBeforeNow = dif.getDay();
                // znaleziono jakies dane, wiec zwroc otwarty plik, rozmiar i ilosc dni
                return true;
              }
              file.close();
            }
        }
      }
      // nie znaleziono danych do wyslania
      return false;
    }
    //!
    //! \brief getLastId ostatnie id
    //! \param bufferFile plik .buffer
    //! \return id
    static int getLastId(std::fstream& bufferFile)
    {
      // jesli nie istenieja pliki to id = -1; przy zapisie id zoanie zwiekszone o 1
      const int idIfNotExist = -1;
      // id zwracane jesli jest logiczny error (poki co przyjmuje takie same) - taka sytuacja nigdy nie bedzie miala miejsca
      const int idError = -1;

      if (bufferFile.is_open())
      {
        SBufferInfo info;
        bufferFile.seekg(0, std::fstream::beg);
        bufferFile.read(reinterpret_cast<char*>(&info), sizeof(info));
        if (info.size == 0)
        {
          // w buforze nie ma wpisu, ale moze jest w .warnings
          // ta sytuacja ma miejsce gdy poprzedni wpis byl warningiem
          const std::string warningPath = getWarningFilePath(0);
          std::ifstream warningFile(warningPath,
              std::ifstream::in | std::ifstream::binary);
          if (warningFile.is_open())
          {
            int size;
            warningFile.read(reinterpret_cast<char*>(&size), sizeof(size));
            if (size == 0)
            {
              return idIfNotExist;
            }
            warningFile.seekg(-sizeof(int) - info.sensors * sizeof(T),
                std::ifstream::end);
            int id;
            warningFile.read(reinterpret_cast<char*>(&id), sizeof(id));
            warningFile.close();
            return id;
          }
          else
          {
            // tutaj nigdy nie powinien dojsc program
            return idError;
          }

        }
        else
        {
          const int offset = sizeof(info)
              + (info.size - 1) * (sizeof(int) + info.sensors * sizeof(T));
          bufferFile.seekg(offset, std::fstream::beg);
          int id;
          bufferFile.read(reinterpret_cast<char*>(&id), sizeof(id));

          return id;
        }
      }
      return idIfNotExist;
    }

    //!
    //! \brief removeIdFromBufferIndexFile usuwa i dodaje id z pliku _(un)confirmed.buffer
    //! \param bufferIndexFile plik indeksow: _(un)confirmed.buffer
    //! \param idToRemove id usuwane
    //! \param idToAdd id dodawane
    //! \return true jesli wszystko ok
    static bool removeIdFromBufferIndexFile(std::fstream& bufferIndexFile,
        const int idToRemove, const int idToAdd)
    {
      std::vector<int> ids;
      readIndexFile(bufferIndexFile, ids, false);

      std::vector<int>::iterator it;
      if ((it = std::find(ids.begin(), ids.end(), idToRemove)) != ids.end())
      {
        // nadpisywana seria ma index w _unconfirmed.buffer
        // wiec nadpisz rowniez _unconfirmed.buffer.
        ids.push_back(idToAdd);
        // rozmiar sie nie zmieni, wiec go omin
        bufferIndexFile.seekg(sizeof(int), std::fstream::beg);
        // zapisz dane - omin pierwszy wpis
        char* flags = reinterpret_cast<char*>(ids.data()) + sizeof(int);
        bufferIndexFile.write(flags, sizeof(int) * (ids.size() - 1));
        return true;
      }
      return false;
    }

  public:

    static void init()
    {
      std::string dataPath = CConfigurationFactory::getInstance()->getDataPath();

      QDir qdir(QString(dataPath.c_str()));
      if (!qdir.exists("data"))
      {
        qdir.mkdir("data");
      }
    }

    //!
    //! \brief The SToConfirm struct serie do potwierdzenia czy dane zapisane na serwerze
    struct SToConfirm
    {
      int id;               //!< id serii
      std::vector<T> data;  //!< seria
    };

    //!
    //! \brief getLastId zwraca ostatnie id serii
    //! \return id
    static int getLastId()
    {
      STime now = CTime::now();
      std::vector<std::time_t> fileList;
      int lastId = -1;
      // odczytaj liste dat zapisanych
      if (readDateListFile(fileList))
      {
        // przechodz od najstarszej
          STime dif = now - STime(*fileList.rbegin());
          std::fstream bufferFile(getBufferFilePath(dif.getDay()),
              std::fstream::in | std::fstream::binary);
          lastId = getLastId(bufferFile);
          bufferFile.close();
          return lastId;
      }
      return lastId;
    }

    //!
    //! \brief saveData zapisuje dane w plikach
    //! \param warning true jesli niebezpieczne
    //! \param sData seria danych
    //! \return  id serii
    static int saveData(const bool warning, const std::vector<T>& sData)
    {
      std::fstream bufferFile(getBufferFilePath(0),
          std::fstream::in | std::fstream::out | std::fstream::binary);

      // sprawdz czy plik istnieje
      if (!bufferFile.is_open())
      {
        // plik nie istnieje
        // zdefragmentuj wczorajsze pliki
        const bool warningYesterday = wasWarning();
        // utworz nowe
        return saveNewFile(warningYesterday, warning, sData);
      }

      if (warning)
      {
        return saveDataWarning(bufferFile, sData);
      }
      else
      {
        return saveDataNotWarning(bufferFile, sData);
      }

    }

    //!
    //! \brief confirm zapisuje potwierdzenia z serwera
    //! \param dayBeforeNow ile dni od dzisiaj dane byly zapisane
    //! \param ids id potwierdzone
    //! \return ilosc potwierdzonych serii w podanym dniu
    static int confirm(const int dayBeforeNow, const std::vector<int>& ids)
    {
      int confirmedData = 0;
      if (ids.size() == 0)
      {
        return confirmedData;
      }
      std::fstream warningUnconfirmedFile(
          getUnconfirmedWarningFilePath(dayBeforeNow),
          std::fstream::in | std::fstream::out | std::fstream::binary);
      std::fstream warningConfirmedFile(
          getConfirmedWarningFilePath(dayBeforeNow),
          std::fstream::in | std::fstream::out | std::fstream::binary);
      std::fstream bufferUnconfirmedFile(
          getUnconfirmedBufferFilePath(dayBeforeNow),
          std::fstream::in | std::fstream::out | std::fstream::binary);
      std::fstream bufferConfirmedFile(getConfirmedBufferFilePath(dayBeforeNow),
          std::fstream::in | std::fstream::out | std::fstream::binary);

      if (warningUnconfirmedFile.is_open() && warningConfirmedFile.is_open()
          && bufferUnconfirmedFile.is_open() && bufferConfirmedFile.is_open())
      {

        // zrob posortowana kopie
        std::vector<int> idsCopy = ids;
        sort(idsCopy.begin(), idsCopy.end());
        // przeczytaj wszystkie niepotwierdzone
        std::vector<SWarningIndex> unconfirmedWarnings;
        readIndexFile(warningUnconfirmedFile, unconfirmedWarnings, false);
        if (unconfirmedWarnings.size() > 0)
        {
          auto it = unconfirmedWarnings.begin();
          std::vector<SWarningIndex> addToConfirmedWarnings;
          do
          {

            if (std::binary_search(idsCopy.begin(), idsCopy.end(), it->id))
            {
              // znaleziony
              addToConfirmedWarnings.push_back(*it);
              it = unconfirmedWarnings.erase(it);
              ++confirmedData;
            }
            else
            {
              ++it;
            }
          }
          while (it != unconfirmedWarnings.end());

          // zapisz cale unconfirmed
          writeIndexFile(warningUnconfirmedFile, unconfirmedWarnings, false);
          // dopisz confirmed
          writeIndexFile(warningConfirmedFile, addToConfirmedWarnings, true);

        }
        // BUFFER
        if (confirmedData != idsCopy.size())
        {
          std::vector<int> unconfirmedBuffer;
          readIndexFile(bufferUnconfirmedFile, unconfirmedBuffer, false);
          if (unconfirmedBuffer.size() > 0)
          {
            auto it2 = unconfirmedBuffer.begin();
            std::vector<int> addToConfirmedBuffer;
            do
            {

              if (std::binary_search(idsCopy.begin(), idsCopy.end(), *it2))
              {
                // znaleziony
                addToConfirmedBuffer.push_back(*it2);
                it2 = unconfirmedBuffer.erase(it2);
                ++confirmedData;
              }
              else
              {
                ++it2;
              }
            }
            while (it2 != unconfirmedBuffer.end());

            // zapisz cale unconfirmed
            writeIndexFile(bufferUnconfirmedFile, unconfirmedBuffer, false);
            // dopisz confirmed
            writeIndexFile(bufferConfirmedFile, addToConfirmedBuffer, true);
          }
        }
      }
      bufferUnconfirmedFile.close();
      bufferConfirmedFile.close();
      warningConfirmedFile.close();
      warningUnconfirmedFile.close();
      return confirmedData;
    }


    //!
    //! \brief getToConfirm zwraca dane do potwiedzenia
    //! \param maxIdsToConfirm ile maksymalnie id
    //! \param toConfirm zwrocone dane
    //! \param dayBeforeNow ile dni od dzisiaj - kiedy pomiary byl zapisany
    //! \return true jesli sa jakies dane, false jesli nie
    static bool getToConfirm(const int maxIdsToConfirm,
        std::vector<SToConfirm>& toConfirm, int& dayBeforeNow)
    {

      int size;
      std::ifstream warningUnconfirmedFile;
      if (findLastUnconfirmedData(dayBeforeNow, warningUnconfirmedFile, size))
      {
        std::ifstream warning(getWarningFilePath(dayBeforeNow),
            std::fstream::in | std::fstream::out | std::fstream::binary);
        SWarningsInfo warningInfo;
        warning.read(reinterpret_cast<char*>(&warningInfo), sizeof(warningInfo));
        warning.close();

        if (warningUnconfirmedFile.is_open())
        {
          // plik istnieje
          int size;
          warningUnconfirmedFile.read(reinterpret_cast<char*>(&size),
              sizeof(size));
          if (size != 0)
          {
            int indicesToRead = size < maxIdsToConfirm ? size : maxIdsToConfirm;

            // sa jakies dane do potwierdzenia
            std::shared_ptr<SWarningIndex> array = getArray<SWarningIndex>(
                indicesToRead);

            SWarningIndex* buf = &*array;
            // odczytywane sa dane z konca
            warningUnconfirmedFile.seekg(-indicesToRead * sizeof(SWarningIndex),
                std::ifstream::end);
            warningUnconfirmedFile.read(reinterpret_cast<char*>(buf),
                indicesToRead * sizeof(SWarningIndex));

            // przed odczytem nalezy uporzadkowac dane
            std::sort(buf, buf + indicesToRead,
                [](const SWarningIndex& s1, const SWarningIndex& s2)
                { return s1.offset < s2.offset;});

            // polaczyc odczyty w wieksze bufory - odczyt nie pojedyczno, ale grupami
            std::vector<std::vector<SWarningIndex>> readBuffer;
            int lastOffset = -1;
            std::vector<SWarningIndex> indicesOneAfterAnother;
            int maxOneAfterAnother = -1;
            for (SWarningIndex* ptr = buf; ptr != buf + indicesToRead; ++ptr)
            {
              if (ptr->offset - lastOffset
                  == sizeof(int) + warningInfo.sensors * sizeof(T)
                  || lastOffset == -1)
              {
                // offsety sa po kolei
                indicesOneAfterAnother.push_back(*ptr);
              }
              else
              {
                // przerwa w offsetach
                readBuffer.push_back(indicesOneAfterAnother);
                if (maxOneAfterAnother < indicesOneAfterAnother.size())
                {
                  maxOneAfterAnother = indicesOneAfterAnother.size();
                }
                indicesOneAfterAnother.clear();
                indicesOneAfterAnother.push_back(*ptr);

              }
              lastOffset = ptr->offset;
            }
            int indOneAfterAnotherSize =
                static_cast<int>(indicesOneAfterAnother.size());

            if (indOneAfterAnotherSize > 0)
            {
              readBuffer.push_back(indicesOneAfterAnother);
              if (maxOneAfterAnother < indOneAfterAnotherSize)
              {
                maxOneAfterAnother = indOneAfterAnotherSize;
              }
            }

            // zaalokuj miejsce na na najwiekszy odczyt
            const int readArrayToConfirmSize = maxOneAfterAnother
                * (sizeof(int) + warningInfo.sensors * sizeof(T));
            std::shared_ptr<char> arrayToConfirm = getArray<char>(
                readArrayToConfirmSize);

            char* bufToConfirm = &*arrayToConfirm;
            // no to odczyt
            std::ifstream warningFile(getWarningFilePath(dayBeforeNow),
                std::fstream::in | std::fstream::out | std::fstream::binary);
            for (std::vector<SWarningIndex>& indicesOneAfterAnother : readBuffer)
            {

              // przesuniecie na offset
              warningFile.seekg(indicesOneAfterAnother.begin()->offset,
                  std::ofstream::beg);
              // odczytanie
              const int sizeToRead = (sizeof(int)
                  + warningInfo.sensors * sizeof(T))
                  * indicesOneAfterAnother.size();
              warningFile.read(reinterpret_cast<char*>(bufToConfirm), sizeToRead);

              for (unsigned int j = 0; j < indicesOneAfterAnother.size(); ++j)
              {
                int* id = reinterpret_cast<int*>(bufToConfirm
                    + j * (sizeof(int) + warningInfo.sensors * sizeof(T)));
                T* buf = reinterpret_cast<T*>(bufToConfirm + sizeof(int)
                    + j * (sizeof(int) + warningInfo.sensors * sizeof(T)));

                SToConfirm toConfirmStruct;
                toConfirmStruct.id = *id;
                toConfirmStruct.data.insert(toConfirmStruct.data.end(), buf,
                    buf + warningInfo.sensors);
                toConfirm.push_back(toConfirmStruct);
              }

            }
            warningFile.close();
            return true;
          }
          warningUnconfirmedFile.close();
        }
      }
      return false;
    }

#ifdef DEBUG_SENSOR_DATA

    struct SSeriesDataTest
    {
      int seriesId;
      std::vector<CSensorData> sensorData;
      int confirmed;
    };

    //!
    //! \brief coutFiles wyswietla pliki z danego dnia
    //! \param dayBeforeNow ile dni od dzisiaj
    static void readFiles(const int dayBeforeNow,
                          std::vector<SSeriesDataTest>& outBuffer,
                          std::vector<SSeriesDataTest>& outWarning)
    {

      std::vector<int> bufferConfirmed;
      std::fstream bufferConfirmedFile(getConfirmedBufferFilePath(dayBeforeNow),
          std::fstream::in);
      if (bufferConfirmedFile.is_open())
      {
        readIndexFile(bufferConfirmedFile, bufferConfirmed, false);
        bufferConfirmedFile.close();
      }

      std::vector<int> bufferUnconfirmed;
      std::fstream bufferUnconfirmedFile(
          getUnconfirmedBufferFilePath(dayBeforeNow), std::fstream::in);
      if (bufferUnconfirmedFile.is_open())
      {
        readIndexFile(bufferUnconfirmedFile, bufferUnconfirmed, false);
        bufferUnconfirmedFile.close();
      }
      std::string bufferPath = getBufferFilePath(dayBeforeNow);
      int (*bufferConverter)(const int& ind) = [](const int& ind){ return ind;};

      readDataFile<SBufferInfo, int>(bufferPath, bufferUnconfirmed, bufferConfirmed, bufferConverter, outBuffer);

      // warnings
      std::vector<SWarningIndex> warningConfirmed;
      std::fstream warningConfirmedFile(
          getConfirmedWarningFilePath(dayBeforeNow), std::fstream::in);
      if (warningConfirmedFile.is_open())
      {
        readIndexFile(warningConfirmedFile, warningConfirmed, false);
        warningConfirmedFile.close();
      }

      std::vector<SWarningIndex> warningUnconfirmed;
      std::fstream warningUnconfirmedFile(
          getUnconfirmedWarningFilePath(dayBeforeNow), std::fstream::in);
      if (warningUnconfirmedFile.is_open())
      {
        readIndexFile(warningUnconfirmedFile, warningUnconfirmed, false);
        warningUnconfirmedFile.close();
      }

      std::string warningPath = getWarningFilePath(dayBeforeNow);
      int (*warningConverter)(const SWarningIndex& ind) = [](const SWarningIndex& ind){ return ind.id;};

      readDataFile<SWarningsInfo, SWarningIndex>(warningPath, warningUnconfirmed, warningConfirmed, warningConverter, outWarning);
    }

    template<typename TInfoType, typename TIndexType>
    static void readDataFile(const std::string& filePath,
                                const std::vector<TIndexType>& unconfirmed,
                                const std::vector<TIndexType>& confirmed,
                                int (*converter)(const TIndexType&),
                                std::vector<SSeriesDataTest>& out)
    {

      std::fstream file(filePath,
          std::fstream::in | std::fstream::binary);
      if (file.is_open())
      {
        TInfoType info;

        file.read(reinterpret_cast<char*>(&info), sizeof(info));
        const int bufferSize = info.size
                               * (sizeof(int) + info.sensors * sizeof(T));

        std::shared_ptr<char> array = getArray<char>(bufferSize);

        char* buf = &*array;
        file.read(reinterpret_cast<char*>(buf), bufferSize);
        for (int i = 0; i < info.size; ++i)
        {
          int* id = reinterpret_cast<int*>(buf
                                           + i * (sizeof(int) + info.sensors * sizeof(T)));
          T* datas = reinterpret_cast<T*>(buf + sizeof(int)
                                          + i * (sizeof(int) + info.sensors * sizeof(T)));

          SSeriesDataTest seriesDataTest;

          seriesDataTest.seriesId = *id;
          // confirmed czy unconfirmed
          seriesDataTest.confirmed = -1;
          if (std::find_if(confirmed.begin(), confirmed.end(),
                           [&](const TIndexType& ind){ return *id = converter(ind);})
              != confirmed.end())
          {
            seriesDataTest.confirmed = 1;
          }
          else if (std::find_if(unconfirmed.begin(), unconfirmed.end(),
                                [&](const TIndexType& ind){ return *id = converter(ind);})
                   != unconfirmed.end())
          {
            seriesDataTest.confirmed = 0;
          }

          std::for_each(datas, datas + info.sensors, [&](T& d){seriesDataTest.sensorData.push_back(d);});
          out.push_back(seriesDataTest);
        }
        file.close();
      }
    }

    static void coutFiles(const int dayBeforeNow)
    {

      void (*fun)(const SSeriesDataTest&) = [](const SSeriesDataTest& data)
      {
        const char* txt[3] = {"confirmed error", "confirmed", "unconfirmed"};
        std::cerr<<"SERIES ID: "<<data.seriesId <<" confirmed: "<<txt[data.confirmed + 1]<<" dataSize; "<<data.sensorData.size()<<std::endl;
      };

      std::vector<SSeriesDataTest> outBuffer;
      std::vector<SSeriesDataTest> outWarning;

      readFiles(dayBeforeNow, outBuffer, outWarning);
      std::cerr<<"---------------------------------------------\n";
      std::cerr<<"-------------------- BUFFERS ----------------\n";
      std::for_each(outBuffer.begin(), outBuffer.end(), fun);
      std::cerr<<"------------------- WARNINGS ----------------\n";
      std::for_each(outWarning.begin(), outWarning.end(), fun);
      std::cerr<<"---------------------------------------------\n";


    }

#endif //DEBUG_SENSOR_DATA
  };

} /* namespace NEngine */

#endif /* SENSORDATAFILEMANAGER_H_ */
