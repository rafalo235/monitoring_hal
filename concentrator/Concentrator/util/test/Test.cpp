#ifdef TEST_ENABLE
#include "Test.h"

namespace NTest
{
std::shared_ptr<std::vector<CTestIndex>> CTestBase::testClasses;
CTestBase::SAssertValue CTestBase::currectTestCaseAsserts;
}
#endif //TEST_ENABLE
