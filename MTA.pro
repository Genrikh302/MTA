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

macx {
    LIBS += -L/opt/local/lib
    QMAKE_CXXFLAGS += -I/opt/local/include
}

LIBS += -lssh2

SOURCES += main.cpp\
    mainwindow.cpp \
    qcallog.cpp \
    qlogdb.cpp \
    filterdialog.cpp \
    cdrtablemodel.cpp \
    qprogrampropertydialog.cpp \
    qchanneltablemodel.cpp \
    qcustomplot.cpp \
    graph.cpp \
    qfiltercombobox.cpp \
    qsshlogindialog.cpp \
    qprogressdialog.cpp \
    progressworker.cpp \
    qsshselectdialog.cpp \
    qsshfile.cpp


HEADERS  += \
    mainwindow.h \
    qcallog.h \
    qlogdb.h \
    filterdialog.h \
    cdrtablemodel.h \
    qprogrampropertydialog.h \
    qchanneltablemodel.h \
    qcustomplot.h \
    graph.h \
    qfiltercombobox.h \
    qsshlogindialog.h \
    qprogressdialog.h \
    progressworker.h \
    qsshselectdialog.h \
    qsshfile.h \
    common.h

HEADERS += stable.h

FORMS    += \
    mainwindow.ui \
    filterdialog.ui \
    qprogrampropertydialog.ui \
    graph.ui \
    qprogress.ui \
    qsshlogindialog.ui \
    qsshselectdialog.ui

