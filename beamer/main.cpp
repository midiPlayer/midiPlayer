#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include "../webSocketConnector/websocketconnector.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<WebSocketConnector>("WebSocketConnector", 1,1, "WebSocketConnector");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
