#-------------------------------------------------
#
# Project created by QtCreator 2015-04-09T21:49:33
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += C++11 precompile_header

OBJECTS_DIR = obj

PRECOMPILED_HEADER  = stable.h

TARGET = MTA
TEMPLATE = app


win32:RC_ICONS += resources/MTA.ico

SOURCES += main.cpp\
    mainwindow.cpp \
    qcallog.cpp \
    qlogdb.cpp \
    filterdialog.cpp \
    cdrtablemodel.cpp \
    qprogrampropertydialog.cpp \
    qchanneltablemodel.cpp \
    qcustomplot.cpp \
    graph.cpp



HEADERS  += \
    mainwindow.h \
    qcallog.h \
    qlogdb.h \
    filterdialog.h \
    cdrtablemodel.h \
    qprogrampropertydialog.h \
    qchanneltablemodel.h \
    qcustomplot.h \
    graph.h


HEADERS += stable.h

FORMS    += \
    mainwindow.ui \
    filterdialog.ui \
    qprogrampropertydialog.ui \
    graph.ui

