#define LOGGER_ENABLE
#define LOGGER_FILE_ENABLE
#include "util/Logger.h"
#include <QCoreApplication>
#include "communication/protocol.h"
#include "communication/HttpThread.h"
#include "communication/Communication.h"
#include "tests/Test.h"

using namespace NProtocol;
using namespace NTest;

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  CTest test;

  test.createProtocol();

  return a.exec();
}
