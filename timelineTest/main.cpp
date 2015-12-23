#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include "rgbwcolor.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<RGBWColor>("RGBWColor", 1,1, "RGBWColor");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
