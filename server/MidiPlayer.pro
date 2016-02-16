#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T10:58:02
#
#-------------------------------------------------

QT       += core multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += websockets

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
    beatscene2.cpp \
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
    diascene.cpp \
    colorbutton.cpp \
    dia.cpp \
    keyframe.cpp \
    devicestate.cpp \
    stopwatch.cpp \
    fileioprovider.cpp \
    musicplayer.cpp \
    whitedevice.cpp \
    monitorio.cpp \
    diascenemonitorcontrol.cpp

HEADERS  += mainwindow.h\
            jackprocessor.h \
    scene.h \
    keyframescene.h \
    beatscene1.h \
    beatscene2.h \
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
    diascene.h \
    colorbutton.h \
    dia.h \
    keyframe.h \
    devicestate.h \
    stopwatch.h \
    fileioprovider.h \
    musicplayer.h \
    whitedevice.h \
    monitorio.h \
    diascenemonitorcontrol.h

FORMS    +=

DISTFILES += \
    other/Icons/MidiPlayer.png
