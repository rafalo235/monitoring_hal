#-------------------------------------------------
#
# Project created by QtCreator 2014-06-03T10:49:31
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = sensor
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libmodbus
