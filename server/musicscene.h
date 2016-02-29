#ifndef MUSICSCENE_H
#define MUSICSCENE_H
#include "scene.h"
#include "musicdevice.h"
#include "websocketserverprovider.h"

class MusicScene : public Scene, public WebSocketServerProvider
{
public:
    MusicScene(QString name, WebSocketServer *ws, QJsonObject serialized = QJsonObject());
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

private:
    MusicDevice player;

};

#endif // MUSICSCENE_H
