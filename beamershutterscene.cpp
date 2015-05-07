#include "beamershutterscene.h"
#include "websocketserver.h"
BeamerShutterScene::BeamerShutterScene(QString name,WebSocketServer *server) : Scene(name),WebSocketServerProvider(server)
{

}

QList<Device> BeamerShutterScene::getLights()
{

}

QList<Device> BeamerShutterScene::getUsedLights()
{

}

void BeamerShutterScene::clientRegistered(QJsonObject msg, int id)
{

}

void BeamerShutterScene::clientUnregistered(QJsonObject msg, int id)
{

}

void BeamerShutterScene::clientMessage(QJsonObject msg, int id)
{

}

QString BeamerShutterScene::getRequestType()
{

}


