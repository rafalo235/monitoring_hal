#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T11:31:59
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Concentrator
CONFIG   += console
CONFIG   -= app_bundle
QT += network
TEMPLATE = app


SOURCES += main.cpp \
    communication/core/HttpThread.cpp \
    util/Logger.cpp \
    util/Cryptography.cpp \
    configuration/core/Configuration.cpp \
    communication/core/Connection.cpp \
    engine/core/CycleMonitor.cpp \
    engine/core/Engine.cpp \
    sensors/core/CSensorsManager.cpp \
    sensors/tests/CSensorsManagerMock.cpp \
    configuration/tests/ConfigurationMock.cpp \
    communication/interfaces/protocol.cpp \
    util/Time.cpp \
    engine/test/SensorDateFileManagerTest.cpp \
    util/test/Test.cpp \
    configuration/tests/CConfigurationTest.cpp

HEADERS += \
    communication/core/protocol.h \
    communication/core/HttpThread.h \
    util/Logger.h \
    util/Cryptography.h \
    configuration/core/SensorConfiguration.h \
    configuration/core/Configuration.h \
    util/QueueThread.h \
    communication/core/ConnectionResult.h \
    communication/core/ByteWrapper.h \
    communication/core/ConnectionTask.h \
    communication/interfaces/IConnection.h \
    communication/interfaces/IConnectionResult.h \
    communication/interfaces/ConnectionFactory.h \
    communication/core/Connection.h \
    configuration/interfaces/IConfiguration.h \
    configuration/interfaces/ConfigurationFactory.h \
    configuration/interfaces/ISensorConfiguration.h \
    engine/core/CycleMonitor.h \
    engine/core/Engine.h \
    sensors/interfaces/ISensorsManager.h \
    sensors/interfaces/SensorsFactory.h \
    sensors/core/CSensorsManager.h \
    communication/interfaces/protocol.h \
    communication/interfaces/protocolUtil.h \
    engine/interfaces/IEngine.h \
    engine/interfaces/EngineFactory.h \
    sensors/tests/CSensorsManagerMock.h \
    configuration/tests/ConfigurationMock.h \
    configuration/tests/SensorConfigurationMock.h \
    engine/core/SensorDataFileManager.h \
    util/Time.h \
    engine/test/SensorDateFileManagerTest.h \
    util/test/Test.h \
    util/Memory.h \
    util/FileHelper.h \
    configuration/tests/ConfigurationTest.h

QMAKE_CXXFLAGS += -std=c++0x

# Wlaczenie/wylaczenie testow
#DEFINES += TEST_ENABLE
DEFINES += TEST_ENABLE_CONFIGURATION_TEST
DEFINES += TEST_ENABLE_SENSOR_DATA_TEST

unix: PKGCONFIG += libmodbus

unix|win32: LIBS += -lmodbus
