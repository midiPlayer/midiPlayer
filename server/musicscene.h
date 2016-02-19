#ifndef MUSICSCENE_H
#define MUSICSCENE_H
#include "scene.h"
#include "musicplayer.h"
#include "websocketserverprovider.h"

class MusicScene : public Scene, public WebSocketServerProvider
{
public:
    MusicScene(QString name, WebSocketServer *ws, QJsonObject serialized = QJsonObject());
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

private:
    MusicPlayer player;

};

#endif // MUSICSCENE_H
