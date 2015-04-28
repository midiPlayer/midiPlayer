#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T12:37:26
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Q5WebSocket
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    echoserver.cpp \
    myqslider.cpp

HEADERS  += mainwindow.h \
    echoserver.h \
    myqslider.h

FORMS    += mainwindow.ui
