#define LOGGER_ENABLE
#define LOGGER_FILE_ENABLE
#include <QCoreApplication>

#include "engine/interfaces/EngineFactory.h"

using namespace NEngine;
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  DEngine engine = CEngineFactory::getInstance();
  if(engine->init())
  {

  }
  return a.exec();
}
