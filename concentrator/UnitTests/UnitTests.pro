#-------------------------------------------------
#
# Project created by QtCreator 2014-06-20T21:22:13
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = SensorDateFileManagerTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += SensorDateFileManagerTest.cpp \
    ../Concentrator/communication/core/ByteWrapper.cpp \
    ../Concentrator/communication/core/Connection.cpp \
    ../Concentrator/communication/core/ConnectionResult.cpp \
    ../Concentrator/communication/core/ConnectionTask.cpp \
    ../Concentrator/communication/core/HttpThread.cpp \
    ../Concentrator/communication/interfaces/protocol.cpp \
    ../Concentrator/configuration/core/Configuration.cpp \
    ../Concentrator/configuration/core/ConfigurationManager.cpp \
    ../Concentrator/configuration/core/SensorConfiguration.cpp \
    ../Concentrator/configuration/tests/ConfigurationMock.cpp \
    ../Concentrator/engine/core/CycleMonitor.cpp \
    ../Concentrator/engine/core/Engine.cpp \
    ../Concentrator/engine/test/SensorDateFileManagerTest.cpp \
    ../Concentrator/sensors/core/CSensorsManager.cpp \
    ../Concentrator/sensors/tests/CSensorsManagerMock.cpp \
    ../Concentrator/util/Cryptography.cpp \
    ../Concentrator/util/Logger.cpp \
    ../Concentrator/util/Time.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../Concentrator/communication/core/ByteWrapper.h \
    ../Concentrator/communication/core/Connection.h \
    ../Concentrator/communication/core/ConnectionResult.h \
    ../Concentrator/communication/core/ConnectionTask.h \
    ../Concentrator/communication/core/HttpThread.h \
    ../Concentrator/communication/interfaces/ConnectionFactory.h \
    ../Concentrator/communication/interfaces/IConnection.h \
    ../Concentrator/communication/interfaces/IConnectionResult.h \
    ../Concentrator/communication/interfaces/protocol.h \
    ../Concentrator/configuration/core/Configuration.h \
    ../Concentrator/configuration/core/ConfigurationManager.h \
    ../Concentrator/configuration/core/SensorConfiguration.h \
    ../Concentrator/configuration/interfaces/ConfigurationFactory.h \
    ../Concentrator/configuration/interfaces/IConfiguration.h \
    ../Concentrator/configuration/interfaces/ISensorConfiguration.h \
    ../Concentrator/configuration/tests/ConfigurationMock.h \
    ../Concentrator/configuration/tests/SensorConfigurationMock.h \
    ../Concentrator/engine/core/CycleMonitor.h \
    ../Concentrator/engine/core/Engine.h \
    ../Concentrator/engine/core/SensorDataFileManager.h \
    ../Concentrator/engine/interfaces/EngineFactory.h \
    ../Concentrator/engine/interfaces/IEngine.h \
    ../Concentrator/engine/test/SensorDateFileManagerTest.h \
    ../Concentrator/sensors/core/CSensorsManager.h \
    ../Concentrator/sensors/interfaces/ISensorsManager.h \
    ../Concentrator/sensors/interfaces/SensorsFactory.h \
    ../Concentrator/sensors/tests/CSensorsManagerMock.h \
    ../Concentrator/util/Cryptography.h \
    ../Concentrator/util/Logger.h \
    ../Concentrator/util/QueueThread.h \
    ../Concentrator/util/Time.h
