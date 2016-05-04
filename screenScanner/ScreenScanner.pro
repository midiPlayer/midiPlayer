#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T16:23:12
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AmbiDMX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    screenanalyzer.cpp \
    ../webSocketConnector/webinterface.cpp \
    ../webSocketConnector/websocketconnector.cpp

HEADERS  += mainwindow.h \
    ../webSocketConnector/webinterface.h \
    ../webSocketConnector/websocketconnector.h \
    screenanalyzer.h

FORMS    += mainwindow.ui
