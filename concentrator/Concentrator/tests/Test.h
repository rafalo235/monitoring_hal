#ifndef TEST_H
#define TEST_H

#include "communication/interfaces/protocol.h"


/*
 * Nie robie konkretnych testow - za duzo kombinowania, za malo czasu.
 * Tutaj tylko umieszczam funkcje, ktore mozna wykorzystac do recznego testowania.
 *
 */
namespace NTest{
  using namespace NProtocol;

  class CTest
  {

  public:
    CTest();

    void createProtocol();

    void saveConfigurationFile();
  };
}
#endif // TEST_H
