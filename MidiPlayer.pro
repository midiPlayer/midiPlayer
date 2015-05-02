#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T10:58:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MidiPlayer
TEMPLATE = app
LIBS += -L C:\\JACK -ljack -laubio

SOURCES += main.cpp\
        mainwindow.cpp\
        jackprocessor.cpp \
    scene.cpp \
    scene1.cpp \
    scene2.cpp \
    scene4.cpp \
    keyframescene.cpp \
    beatscene1.cpp \
    overlayscene.cpp

HEADERS  += mainwindow.h\
            jackprocessor.h \
    scene.h \
    scene1.h \
    scene2.h \
    scene4.h \
    keyframescene.h \
    beatscene1.h \
    overlayscene.h

FORMS    += mainwindow.ui
