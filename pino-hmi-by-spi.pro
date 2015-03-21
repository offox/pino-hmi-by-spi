#-------------------------------------------------
#
# Project created by QtCreator 2015-03-21T09:49:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pino-hmi-by-spi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    spicomm.cpp

HEADERS  += mainwindow.h \
    spicomm.h

FORMS    += mainwindow.ui
