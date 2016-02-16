TEMPLATE = app

QT += qml quick widgets multimedia websockets

SOURCES += main.cpp \
    mediamanager.cpp \
    ../webSocketConnector/webinterface.cpp \
    ../webSocketConnector/websocketconnector.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    mediamanager.h \
    ../webSocketConnector/webinterface.h \
    ../webSocketConnector/websocketconnector.h
