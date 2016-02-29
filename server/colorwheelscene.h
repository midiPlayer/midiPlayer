#ifndef COLORWHEELSCENE_H
#define COLORWHEELSCENE_H

#include "scene.h"
#include "websocketserverprovider.h"
#include "device.h"
#include "time.h"
#include "trigger.h"
#include "filtervirtualdevicemanager.h"

class ColorWheelScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT

public:
    ColorWheelScene(VirtualDeviceManager *manager,WebSocketServer *ws, JackProcessor *jackP,  QString name, QJsonObject serialized = QJsonObject());

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
    void updateDevices();
private:
    struct PositionedDevice{
        QSharedPointer<Device> d;
        float angle;
    };
    QList<PositionedDevice> positionedDevices;
    QTime stopwatch;
    QTime triggerStopwatch;
    Trigger trigger;
    float basicSpeed;//rounds/s
    float bumpSpeed;//rounds/s
    float anchor;
    float duration;
    double getSpeed(double time);
    double getDelta();
    QColor normalizeColor(QColor color);

    FilterVirtualDeviceManager filterDevManager;
};

#endif // COLORWHEELSCENE_H
