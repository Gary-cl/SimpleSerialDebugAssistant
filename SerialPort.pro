#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T18:53:12
#
#-------------------------------------------------


QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPort
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialportrun.cpp

HEADERS  += mainwindow.h \
    serialportrun.h

FORMS    += mainwindow.ui
