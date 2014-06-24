#ifndef TEST_H_
#define TEST_H_

#ifdef TEST_ENABLE

#include <vector>
#include <memory>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <thread>
//! tworza unikatowa nazwy z prefiksem
#define CONCAT(a, b) a ## b
#define CONCAT2(a, b) CONCAT(a, b)
#define CONCAT3(a, b, c) CONCAT2(CONCAT2(a, b), c)

#define UNIQUE_NAME(prefix) CONCAT2(prefix, __LINE__)

//! \brief Dodaje klase do testow
#define ADD_TEST(className) std::shared_ptr<NTest::CTestHelper<className>> UNIQUE_NAME(className) (new NTest::CTestHelper<className>(__FILE__, #className));

//! \brief AsertEQ
#define assertEq(t1, t2) assertEqFun(t1, t2, __FILE__, __LINE__);

//! \brief namespace testow
//! Przyklad uzycia
//!
//! class CTest3: public NTest::CTest<CTest3>
//! {
//! public:
//!   CTest3()
//!   {
//!     addTestCase("testCas2", &CTest3::testCase2);
//!     addTestCase("testCase1", &CTest3::testCase1);
//!   }
//!
//!   void testCase2()
//!   {
//!     std::cout << "testCase2\n";
//!     assertEq(1, 2);
//!   }
//!   void testCase1()
//!   {
//!     std::cout << "testCase1\n";
//!     assertEq(1, 1);
//!   }
//!   virtual void setUp()
//!   {
//!     std::cout << "CTest2 setUp\n";
//!   }
//!   virtual void tearDown()
//!   {
//!     std::cout << "CTest2 tearDown\n";
//!   }
//! };
//!
//! do *.cpp:
//! ADD_TEST(CTest3);
namespace NTest
{
  class CTestBase;

  //!
  //! \brief The CTestIndex class zawiera informacje o klasie testowej
  class CTestIndex
  {
    //! \brief sciezka do klasy testowej
    std::string filePath;
    //! \brief nazwa klasy testowej
    std::string className;
    //! \brief wskaznik do obiektu klasy testowej
    std::shared_ptr<CTestBase> testClass;

  public:

    //!
    //! \brief CTestIndex Konstruktor
    //! \param filePath1 siezka do klasy testowej
    //! \param className1 nazwa klasy testowej
    //! \param testClass1 wskaznik do obiektu klasy testowej
    CTestIndex(const std::string& filePath1,
               const std::string& className1,
               const std::shared_ptr<CTestBase> testClass1) :
                 filePath(filePath1),
                 className(className1),
                 testClass(testClass1)
    {}

    //!
    //! \brief getFilePath zwraca sciezke do klasy testowej
    //! \return sciezka do klasy testowej
    std::string getFilePath() const
    {
      return filePath;
    }

    //!
    //! \brief getClassName zwraca nazwe klasy testowej
    //! \return nazwa klasy testowej
    std::string getClassName() const
    {
      return className;
    }

    //!
    //! \brief getTestClass zwraca smart pointer do klasy testowej
    //! \return wskanik do klasy testowej
    std::shared_ptr<CTestBase> getTestClass() const
    {
      return testClass;
    }
  };

  //!
  //! \brief The CTestBase class klasa bazowa wszystkich testow. Zawiera liste wskaznikow do wszystkich klas testowych
  class CTestBase
  {

  protected:
    //! \brief Wektor wskaznikow do wszystkich klas testowych
    //static std::vector<CTestIndex> testClasses;
    static std::shared_ptr<std::vector<CTestIndex>> testClasses;
    //!
    //! \brief beforeClass funkcja uruchamiania na samym poczatku klasy testowej przed testwami (adnotacja BeforeClass)
    virtual void beforeClass()
    {

    }

    //! \brief setUp funkcja uruchamiana jest przed kazdym testem (adnotacja Before)
    virtual void setUp()
    {

    }

    //! \brief tearDown funkcja uruchamiana jest po kazdym tescie (adnotacja After)
    virtual void tearDown()
    {

    }

    //! \brief afterClass funkcja jest uruchamiana po wszystkich testach z jednej klasy testowej (adnotacja AfterClass)
    virtual void afterClass()
    {

    }

    //! \brief runTest uruchamia testy z klasy testowej
    virtual void runTests() = 0;


  public:

    virtual ~CTestBase() = default;

    //!
    //! \brief print wyswietla podane teksty
    //! \param b znak tla
    //! \param txt1 pierwszy napis
    //! \param txt2 drugi napis
    static void print(const char b, const std::string* txt1 = nullptr,
        const std::string* txt2 = nullptr)
    {

      static const int lineWidth = 80;
      char signs[lineWidth + 2];
      char* ptr = signs;
      if (txt1 != nullptr)
      {
        int txtSize1 = txt1->size();
        txtSize1 = txtSize1 > lineWidth ? lineWidth : txtSize1;
        if (txt2 != nullptr)
        {
          static const char* separator = ": ";
          int separatorSize = 2;
          int txtSize2 = txt2->size();
          if (txtSize1 + separatorSize > lineWidth)
          {
            separatorSize = 0;
            txtSize2 = 0;
          }
          else if (txtSize1 + txtSize2 + separatorSize > lineWidth)
          {
            txtSize2 = lineWidth - txtSize1 - separatorSize;
          }
          int offset = (lineWidth - txtSize1 - txtSize2 - separatorSize) / 2;
          offset = offset < 0 ? 0 : offset;
          std::fill_n(ptr, offset, b);
          ptr += offset;
          memcpy(ptr, txt1->data(), txtSize1);
          ptr += txtSize1;
          memcpy(ptr, separator, separatorSize);
          ptr += separatorSize;
          memcpy(ptr, txt2->data(), txtSize2);
          ptr += txtSize2;
          std::fill_n(ptr, lineWidth - (ptr - signs), b);
        }
        else
        {
          int offset = (lineWidth - txtSize1) / 2;
          offset = offset < 0 ? 0 : offset;
          std::fill_n(ptr, offset, b);
          ptr += offset;
          memcpy(ptr, txt1->data(), txtSize1);
          ptr += txtSize1;
          std::fill_n(ptr, lineWidth - (ptr - signs), b);
        }
      }
      else
      {
        std::fill(ptr, ptr + lineWidth, b);
      }
      signs[lineWidth] = '\n';
      signs[lineWidth + 1] = '\0';

      std::cout << signs;

    }
    //!
    //! \brief addInstance dodaje klase testowa do listy
    //! \param ptr wskaznik do klasy testowej
    static void addInstance(const CTestIndex& ptr)
    {
      if (testClasses.use_count() == 0)
      {
        testClasses.reset(new std::vector<CTestIndex>());
      }
      testClasses->push_back(ptr);
    }

    //!
    //! \brief runAllTests uruchamia wszystkie testy
    static void runAllTests()
    {
      const char sep = '*';
      const char sep2 = ' ';
      const std::string fileTxt("FILE");
      const std::string classNameTxt("ClassName");
      for (CTestIndex& i : *testClasses)
      {
        print(sep);
        std::string filePath = i.getFilePath();
        std::string className = i.getClassName();
        print(sep2, &fileTxt, &filePath);
        print(sep2, &classNameTxt, &className);
        print(sep);
        std::shared_ptr<CTestBase> ptr = i.getTestClass();
        ptr->beforeClass();
        ptr->runTests();
        ptr->afterClass();
        print(sep);

      }
    }

    //!
    //! \brief assertFun asertEquals
    //! \param t1 wartosc lewa - expected
    //! \param t2 wartosc prawa - actual
    //! \param file sciezka do pliku, gdzie uzyto asercji
    //! \param line nr linie, w ktorej uzyto asercji
    template<typename T1, typename T2>
    static void assertEqFun(const T1& t1, const T2& t2, const char* file,
        const int line)
    {
      if (t1 != t2)
      {
        std::cout << "assert failed. File: " << file << " line: " << line
            << std::endl;
        ;
      }
    }
  };


  //!
  //! \brief The CTest class klasa bazowa testow jednostkowych, ktora zawiera lista wskazniko do funkcji realizujacych testCase'y
  template<typename T>
  class CTest: public CTestBase
  {
    //! \brief Typedef wskaznika do funkcji realizujacych testCasey
    typedef void (T::*DTestCase)();

    //!
    //! \brief The STestCase struct informacje o testCasie
    struct STestCase
    {
      //! \brief nazwa testCase'a
      std::string caseName;
      //! \brief wskaznik do funkcji testCase'a
      DTestCase testCase;
    };

    //! \brief wektor wskaznikow do funkcji testCase
    std::vector<STestCase> testCases;

  protected:

    //!
    //! \brief addTestCase dodaje funkcje testCase'a
    //! \param caseName1 nazwa testCase'a
    //! \param testCase wskaznik do testCase\a
    void addTestCase(const char* caseName1, DTestCase testCase)
    {
      testCases.push_back(STestCase
      { caseName1, testCase });
    }

    //!
    //! \brief runTests uruchamia testy klasy testowej
    virtual void runTests()
    {
      static const char sep = '-';
      static const std::string testCaseTxt("TestCase");

      for (const STestCase& j : testCases)
      {
        print(sep, &testCaseTxt, &j.caseName);
        setUp();
        T* buf = reinterpret_cast<T*>(this);
        try
        {

          (buf->*(j.testCase))();

        }
        catch (...)
        {

        }

        tearDown();
        print(sep);
      }
    }

  public:
    virtual ~CTest() = default;

  };


  //!
  //! \brief The CTestHelper class Klasa wspomagajaca. Dodaje klase testowa do listy klas testowych
  template<typename T>
  class CTestHelper
  {
  public:

    //!
    //! \brief CTestHelper dodaje klase testowa do listy klas testowych
    //! \param filePath1 sciezka do klasy testowej
    //! \param className1 nazwa klasy testowej
    CTestHelper(const char* filePath1, const char* className1)
    {
      std::shared_ptr<T> ptr(new T());
      std::shared_ptr<CTestBase> basePtr = std::dynamic_pointer_cast<CTestBase>(
          ptr);

      CTestIndex index(filePath1, className1, basePtr);
      CTestBase::addInstance(index);
    }
  };
}
#endif // TEST_ENABLE
#endif /* TEST_H_ */
