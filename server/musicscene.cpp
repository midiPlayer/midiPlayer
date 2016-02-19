#include "musicscene.h"
#include "websocketserver.h"

#define KEY_PLAYER "player"

MusicScene::MusicScene(QString name, WebSocketServer *ws, QJsonObject serialized):
    Scene(name,serialized), WebSocketServerProvider(ws),player(ws,serialized.value(KEY_PLAYER).toObject())
{
    ws->registerProvider(this);
}

QList<Device> MusicScene::getLights()
{
    return QList<Device>();
}

QList<Device> MusicScene::getUsedLights()
{
    return QList<Device>();
}

void MusicScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject ret;
    ret.insert("player",player.providerId);
    sendMsg(ret,id,true);
}

void MusicScene::clientUnregistered(QJsonObject msg, int id)
{

}

void MusicScene::clientMessage(QJsonObject msg, int id)
{

}

QString MusicScene::getRequestType()
{
    return getSceneTypeStringStaticaly();
}

void MusicScene::stop()
{
    player.stop();
}

void MusicScene::start()
{
    player.play(0);
}

QJsonObject MusicScene::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_PLAYER,player.serialize());
    return serializeScene(ret);
}

QString MusicScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString MusicScene::getSceneTypeStringStaticaly()
{
    return "musicScene";
}


