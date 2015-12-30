#include "fileioprovider.h"
#include "websocketserver.h"
#include <QJsonObject>
#include <QDebug>
FileIOProvider::FileIOProvider(WebSocketServer *ws, QSharedPointer<DiaScene> *mainSceneP) : WebSocketServerProvider(ws),
  mainScene(mainSceneP)
{
    ws->registerProvider(this);
}

void FileIOProvider::clientRegistered(QJsonObject msg, int id)
{

}

void FileIOProvider::clientUnregistered(QJsonObject msg, int clientIdCounter)
{

}

void FileIOProvider::clientMessage(QJsonObject msg, int id)
{
    qDebug() << "send Meddage";
    if(msg.contains("save")){
        QJsonObject ret;
        ret.insert("export", mainScene->data()->serialize());
        sendMsg(ret,id,false);
    }
}

QString FileIOProvider::getRequestType()
{
    return "FileIO";
}
