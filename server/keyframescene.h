#ifndef KEYFRAMESCENE_H
#define KEYFRAMESCENE_H
#include "scene.h"
#include "stopwatch.h"
#include <QList>
#include "device.h"
#include "keyframe.h"
#include "websocketserverprovider.h"
#include <QSharedPointer>
#include "musicdevice.h"
#include "filtervirtualdevicemanager.h"

class KeyFrameScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT

public:
    KeyFrameScene(VirtualDeviceManager *manager,QString name, WebSocketServer *ws, QJsonObject serialized = QJsonObject());
    QMap<QString,QSharedPointer<DeviceState> > getDeviceState();
    void start();
    void stop();
    QJsonObject serialize();

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();

public slots:
    void removeKeyframe(Keyframe * which);
    void handleResume();
    void handleTimeChanged();
    void devicesChanged();

private:
    Stopwatch watch;
    QList<QSharedPointer<Keyframe> > keyframes;
    QJsonArray getLampsJson();
    QJsonObject getLampJson(QSharedPointer<Device> dev);
    WebSocketServer* wss;
    void clear(QString devId);
    MusicDevice musicPlayer;
    FilterVirtualDeviceManager filterVDevManager;

};

#endif // KEYFRAMESCENE_H
