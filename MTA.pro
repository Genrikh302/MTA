#-------------------------------------------------
#
# Project created by QtCreator 2015-04-09T21:49:33
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11 precompile_header

OBJECTS_DIR = obj

TARGET = MTA
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    qcallog.cpp \
    qlogdb.cpp

HEADERS  += \
    mainwindow.h \
    qcallog.h \
    qlogdb.h

HEADERS += stable.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    callstatistics.qrc
