TEMPLATE = app

QT += qml quick widgets websockets

SOURCES += \
    ../webSocketConnector/webinterface.cpp \
    ../webSocketConnector/websocketconnector.cpp \
    main.cpp \
    rgbwcolor.cpp \
    fileio.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    icons/move.png

HEADERS += \
../webSocketConnector/webinterface.h \
    ../webSocketConnector/websocketconnector.h \
    rgbwcolor.h \
    fileio.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

