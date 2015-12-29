#ifndef KEYFRAMESCENE_H
#define KEYFRAMESCENE_H
#include "scene.h"
#include "stopwatch.h"
#include <QList>
#include "device.h"
#include "keyframe.h"
#include "websocketserverprovider.h"
#include <QSharedPointer>

class KeyFrameScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT

public:
    KeyFrameScene(QList<Device> avDev,QString name, WebSocketServer *ws, QJsonObject serialized = QJsonObject());
    QList<Device>getLights();
    QList<Device> getUsedLights();
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
private:
    Stopwatch watch;
    QList<Device> myDevs;
    QList<QSharedPointer<Keyframe> > keyframes;
    QJsonArray getLampsJson();
    QJsonObject getLampJson(Device dev);
    WebSocketServer* wss;

};

#endif // KEYFRAMESCENE_H
