#include <QString>
#include <QtTest>

class CSensorDateFileManagerTest : public QObject
{
    Q_OBJECT

public:
    CSensorDateFileManagerTest();

private Q_SLOTS:
    void moreThanSeriesAround();
};

CSensorDateFileManagerTest::CSensorDateFileManagerTest()
{
}

void CSensorDateFileManagerTest::moreThanSeriesAround()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(CSensorDateFileManagerTest)

#include "SensorDateFileManagerTest.moc"
