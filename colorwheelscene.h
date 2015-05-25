#ifndef COLORWHEELSCENE_H
#define COLORWHEELSCENE_H

#include "scene.h"
#include "websocketserverprovider.h"
#include "device.h"
#include "time.h"
#include "trigger.h"
class ColorWheelScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT
public:
    ColorWheelScene(QList<Device> avDev,WebSocketServer *ws, JackProcessor *jackP,  QString name, QJsonObject serialized = QJsonObject());
    QList<Device> getLights();
    QList<Device> getUsedLights();
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    void stop();
    void start();
    QJsonObject serialize();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
public slots:
    void triggered();
private:
    struct PositionedDevice{
        Device d;
        float angle;
    };
    QList<Device> usedDevices;
    QList<PositionedDevice> positionedDevices;
    QTime stopwatch;
    QTime triggerStopwatch;
    Trigger trigger;
    float basicSpeed;//rounds/s
    float maxSpeed;//rounds/s
    float anchor;
    float duration;
    double getSpeed(double time);
    double getDelta();
    QColor normalizeColor(QColor color);
};

#endif // COLORWHEELSCENE_H
