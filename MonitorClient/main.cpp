#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include "mediamanager.h"
#include "../webSocketConnector/websocketconnector.h"
#include <QQmlContext>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<MediaManager>("MediaManager", 1,1, "MediaManager");
    qmlRegisterType<WebSocketConnector>("WebSocketConnector", 1,1, "WebSocketConnector");

    QQmlApplicationEngine engine;

    if(argc != 3){
        qDebug() << "ERROR wrong parameter count!";
        qDebug() << "Use: ws://server.ip:8888 monitorName";
        exit(1);
    }

    QQmlContext* ctx = engine.rootContext();
    ctx->setContextProperty("urlParam", argc >= 2 ? argv[1] : "");
    ctx->setContextProperty("nameParam", argc >= 3 ? argv[2] : "");

    ctx->setContextProperty("desktopWidth", app.desktop()->geometry().width());
    ctx->setContextProperty("desktopHeight", app.desktop()->geometry().height());


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

