#-------------------------------------------------
#
# Project created by QtCreator 2014-06-03T10:49:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sensor
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    sensor.cpp \
    valuegenerator.cpp

QMAKE_CXXFLAGS += -std=c++0x

unix: PKGCONFIG += libmodbus

unix|win32: LIBS += -lmodbus

FORMS += \
    mainwindow.ui

HEADERS += mainwindow.h \
    sensor.h \
    valuegenerator.h
