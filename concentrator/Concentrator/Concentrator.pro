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
    communication/Communication.cpp \
    communication/HttpThread.cpp \
    util/Logger.cpp \
    util/Cryptography.cpp \
    configuration/SensorConfiguration.cpp \
    configuration/Configuration.cpp \
    configuration/ConfigurationManager.cpp \
    tests/Test.cpp

HEADERS += \
    communication/protocol.h \
    communication/Communication.h \
    communication/HttpThread.h \
    util/Logger.h \
    util/Cryptography.h \
    configuration/SensorConfiguration.h \
    configuration/Configuration.h \
    configuration/ConfigurationManager.h \
    tests/Test.h

QMAKE_CXXFLAGS += -std=c++11
