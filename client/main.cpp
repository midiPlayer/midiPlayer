#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include "../webSocketConnector/websocketconnector.h"
#include "rgbwcolor.h"
#include "fileio.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<WebSocketConnector>("WebSocketConnector", 1,1, "WebSocketConnector");
    qmlRegisterType<RGBWColor>("RGBWColor", 1,1, "RGBWColor");
    qmlRegisterType<FileIO>("FileIO", 1,0, "FileIO");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    app.setOrganizationName("fdg");
    app.setOrganizationDomain("fdg-ab.de");
    app.setApplicationName("light control");
    return app.exec();
}
