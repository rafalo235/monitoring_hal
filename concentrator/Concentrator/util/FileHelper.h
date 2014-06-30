#ifndef FILEHELPER_H
#define FILEHELPER_H

#include "configuration/interfaces/ConfigurationFactory.h"
#include <QString>
#include <QDir>
namespace NUtil
{
class CFileHelper
{
public:
  CFileHelper() = delete;

  //!
  //! \brief removeDir funkcja usuwa katalog i rekurencyjnie wszystko co w nim jest
  //! \param dirName sciezka do katalogu
  //! \return true jesli katalog podany zostal usuniety
  static bool removeDir(const QString& dirName)
  {
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
      Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (info.isDir()) {
          result = removeDir(info.absoluteFilePath());
        }
        else {
          result = QFile::remove(info.absoluteFilePath());
        }

        if (!result) {
          return result;
        }
      }
      result = dir.rmdir(dirName);
    }
    return result;
  }

  //!
  //! \brief recreateDataDirectiory usuwa i tworzy katalog dla danych
  static void recreateDataDirectiory()
  {
    std::string dataPath = NEngine::CConfigurationFactory::getInstance()->getDataPath();
    QDir qdir(QString(dataPath.c_str()));
    removeDir(qdir.absoluteFilePath("data"));
    if (!qdir.exists("data"))
    {
      qdir.mkdir("data");
    }
  }
};
}

#endif // FILEHELPER_H
