#ifndef COLORWAVESCENE_H
#define COLORWAVESCENE_H
#include "scene.h"
#include "websocketserverprovider.h"
#include "trigger.h"
#include <QTime>
#include "colorbutton.h"
class ColorWaveScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT
public:
    ColorWaveScene(QList<Device> avDev,WebSocketServer *ws, JackProcessor *jackP,  QString name, QJsonObject serialized = QJsonObject());
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
    Trigger trigger;
    QList<Device> usedDevices;
    QList<Device> onState;
    QTime beatStopwatch;
    bool isRunning;
    int centerDevPos;
    QVector3D center;
    float getPercentageForDistance(float distance);

    float activeDistance;// in meters
    float speed;// in m/s

    ColorButton colorButton;
};

#endif // COLORWAVESCENE_H
