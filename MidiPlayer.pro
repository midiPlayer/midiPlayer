#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T10:58:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets websockets

TARGET = MidiPlayer
TEMPLATE = app
LIBS += -L C:\\JACK -ljack -laubio
LIBS      += -L/usr/local/lib -lolaserver -lola -lolacommon -lprotobuf

SOURCES += main.cpp\
        mainwindow.cpp\
        jackprocessor.cpp \
    scene.cpp \
    keyframescene.cpp \
    beatscene1.cpp \
    overlayscene.cpp \
    discoscene.cpp \
    device.cpp \
    fusionscene.cpp \
    websocketserver.cpp \
    websocketserverprovider.cpp \
    beamershutterscene.cpp \
    beamerdeviceprovider.cpp \
    outputdevice.cpp \
    remotebeat.cpp \
    flashscene.cpp \
    trigger.cpp \
    beamerdevice.cpp \
    circularbeamershutterscene.cpp \
    beamershutterscenemanager.cpp \
    serializable.cpp \
    discosubscene.cpp \
    scenebuilder.cpp \
    oladeviceprovider.cpp \
    colorscene.cpp \
    colorwheelscene.cpp \
    colorwavescene.cpp \
    colorbutton.cpp

HEADERS  += mainwindow.h\
            jackprocessor.h \
    scene.h \
    keyframescene.h \
    beatscene1.h \
    overlayscene.h \
    discoscene.h \
    device.h \
    fusionscene.h \
    websocketserver.h \
    websocketserverprovider.h \
    beamershutterscene.h \
    beamerdeviceprovider.h \
    outputdevice.h \
    remotebeat.h \
    flashscene.h \
    trigger.h \
    beamerdevice.h \
    circularbeamershutterscene.h \
    beamershutterscenemanager.h \
    serializable.h \
    discosubscene.h \
    scenebuilder.h \
    oladeviceprovider.h \
    colorscene.h \
    colorwheelscene.h \
    colorwavescene.h \
    colorbutton.h

FORMS    += mainwindow.ui
