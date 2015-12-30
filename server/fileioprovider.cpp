#include "fileioprovider.h"
#include "websocketserver.h"
#include <QJsonObject>
#include <QDebug>
#include "mainwindow.h"
FileIOProvider::FileIOProvider(WebSocketServer *ws, QSharedPointer<DiaScene> *mainSceneP, MainWindow *mainwindowP) : WebSocketServerProvider(ws),
  mainScene(mainSceneP), mainwindow(mainwindowP)
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
    if(msg.contains("save")){
        QJsonObject ret;
        ret.insert("export", mainScene->data()->serialize());
        sendMsg(ret,id,false);
    }

    if(msg.contains("open")){
        QJsonObject load = msg.value("open").toObject();
        mainwindow->loadScenes(load);
    }
}

QString FileIOProvider::getRequestType()
{
    return "FileIO";
}
