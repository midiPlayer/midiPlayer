#include <QApplication>
#include <QQmlApplicationEngine>
#include "webinterface.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WebInterface::init();
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
