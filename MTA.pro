#-------------------------------------------------
#
# Project created by QtCreator 2015-04-09T21:49:33
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MTA
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    qcallog.cpp \
    qlogdb.cpp \
    mytablemodel.cpp \
    filterdialog.cpp

HEADERS  += \
    mainwindow.h \
    qcallog.h \
    qlogdb.h \
    mytablemodel.h \
    filterdialog.h

FORMS    += \
    mainwindow.ui \
    filterdialog.ui

RESOURCES += \
    callstatistics.qrc
