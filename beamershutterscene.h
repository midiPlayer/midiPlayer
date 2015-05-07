#ifndef BEAMERSHUTTERSCENE_H
#define BEAMERSHUTTERSCENE_H
#include "scene.h"
#include "websocketserverprovider.h"
class WebSocketServer;
class BeamerShutterScene : public Scene, public WebSocketServerProvider
{
public:
    BeamerShutterScene(QString name,WebSocketServer *server);
    QList<Device> getLights();
    QList<Device> getUsedLights();

    void clientRegistered(QJsonObject msg,int id);
    void clientUnregistered(QJsonObject msg,int id);
    void clientMessage(QJsonObject msg,int id);
    QString getRequestType();
};

#endif // BEAMERSHUTTERSCENE_H
