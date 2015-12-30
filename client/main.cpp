#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include "../webSocketConnector/websocketconnector.h"
#include "rgbwcolor.h"
#include "fileoutput.h"
#include "fileinput.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<WebSocketConnector>("WebSocketConnector", 1,1, "WebSocketConnector");
    qmlRegisterType<RGBWColor>("RGBWColor", 1,1, "RGBWColor");
    qmlRegisterType<FileOutput>("FileOutput", 1,0, "FileOutput");
    qmlRegisterType<FileInput>("FileInput", 1,0, "FileInput");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    app.setOrganizationName("fdg");
    app.setOrganizationDomain("fdg-ab.de");
    app.setApplicationName("light control");
    return app.exec();
}
