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
    keyframe.cpp \
    stopwatch.cpp \
    fileioprovider.cpp \
    whitedevice.cpp \
    monitorio.cpp \
    diascenemonitorcontrol.cpp \
    musicscene.cpp \
    virtualdevicemanager.cpp \
    channeldevicestate.cpp \
    devicestate.cpp \
    channeldevice.cpp \
    defaultvirtualdevicemanager.cpp \
    dia.cpp \
    filtervirtualdevicemanager.cpp \
    hardcodedvirtualdevicemanager.cpp \
    musicdevice.cpp \
    musicdevicestate.cpp

HEADERS  += mainwindow.h\
            jackprocessor.h \
    scene.h \
    keyframescene.h \
    beatscene1.h \
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
    stopwatch.h \
    fileioprovider.h \
    whitedevice.h \
    monitorio.h \
    diascenemonitorcontrol.h \
    musicscene.h \
    virtualdevicemanager.h \
    channeldevicestate.h \
    devicestate.h \
    channeldevice.h \
    defaultvirtualdevicemanager.h \
    filtervirtualdevicemanager.h \
    hardcodedvirtualdevicemanager.h \
    musicdevice.h \
    musicdevicestate.h

FORMS    +=

DISTFILES += \
    other/Icons/MidiPlayer.png
