#ifndef COLORWAVESCENE_H
#define COLORWAVESCENE_H
#include "scene.h"
#include "websocketserverprovider.h"
#include "trigger.h"
#include <QTime>
#include "colorbutton.h"
#include "filtervirtualdevicemanager.h"

class ColorWaveScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT

public:
    ColorWaveScene(VirtualDeviceManager *manager,WebSocketServer *ws, JackProcessor *jackP,  QString name, QJsonObject serialized = QJsonObject());
    QMap<QString,QSharedPointer<DeviceState> > getDeviceState();
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
    void reinitDevices();
    void reinitColors();

private:
    Trigger trigger;
    QMap<QString, QSharedPointer<DeviceState> > onState;
    QTime beatStopwatch;
    bool isRunning;
    int centerDevPos;
    QVector3D center;
    float getPercentageForDistance(float distance);

    float activeRadius;// in meters
    float speed;// in m/s

    ColorButton colorButton;

    FilterVirtualDeviceManager filterDeviceManager;
};

#endif // COLORWAVESCENE_H
