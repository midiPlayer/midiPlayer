#include "diascene.h"
#include "websocketserver.h"
#include <QJsonArray>
DiaScene::DiaScene(QList<Device> avDev, WebSocketServer *ws,
                   JackProcessor *jackP,SceneBuilder builderP, QString name, QJsonObject serialized):
                Scene(name,serialized),WebSocketServerProvider(ws),availableDevices(avDev),
                current(-1),fadingTo(-1),fadeTimer(),fusion("fusion"),wss(ws),builder(builderP)
{
    ws->registerProvider(this);
}

QList<Device> DiaScene::getLights()
{
    fusion.reset();

    if(current != -1)
        fusion.import(scenes.at(current).data()->scene.data());
    if(fadingTo != -1){
        QSharedPointer<Dia> fadingToDia  =scenes.at(fadingTo);
        float percentage = fadeTimer.elapsed()/fadingToDia.data()->fadeInDuration;
        if(percentage> 1){
            percentage = 1;
            current = fadingTo;
        }
        fusion.fusion(fadingToDia.data()->scene,Device::AV,percentage);
    }

    return fusion.getLights();
}

QList<Device> DiaScene::getUsedLights()
{
    return availableDevices;
}

void DiaScene::clientRegistered(QJsonObject msg, int id)
{
    sendMsg(getState(true),id,false);
}

void DiaScene::clientUnregistered(QJsonObject msg, int id)
{

}

void DiaScene::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("resetScene")){
        if(current != -1){
            scenes.at(current).data()->scene.data()->stop();
            scenes.at(current).data()->scene.data()->start();
        }
        if(fadingTo != -1){
            scenes.at(fadingTo).data()->scene.data()->stop();
            scenes.at(fadingTo).data()->scene.data()->start();
        }
        sendMsgButNotTo(msg,id,false);
    }
    if(msg.contains("nextScene")){
        if(scenes.length() != 0 && current < scenes.length() -1){
            fadeTimer.restart();
            if(fadingTo != -1)
                scenes.at(fadingTo).data()->scene.data()->stop();
            scenes.at(current+1).data()->scene.data()->start();
            fadingTo = current + 1;
        }
        sendMsg(getState(false),false);
    }
    if(msg.contains("currentScene")){
        int newId = msg.value("currentScene").toInt(-1);
        if(newId != -1 && newId < scenes.length()){
            fadeTimer.restart();
            if(fadingTo != -1)
                scenes.at(fadingTo).data()->scene.data()->stop();
            scenes.at(newId).data()->scene.data()->start();
            fadingTo = newId;
        }
        sendMsgButNotTo(getState(false),id,false);
    }
    if(msg.contains("prevScene")){
        if(scenes.length() != 0 && current > 0){
            fadeTimer.restart();
            if(fadingTo != -1)
                scenes.at(fadingTo).data()->scene.data()->stop();
            scenes.at(current-1).data()->scene.data()->start();
            fadingTo = current - 1;
        }
        sendMsg(getState(false),false);
    }

    if(msg.contains("addScene")){
        QJsonObject addCmd = msg.value("addScene").toObject();
        QSharedPointer<Scene> newScene = builder.build(addCmd.value("type").toString(),addCmd.value("name").toString());
        if(newScene.isNull()){
           qDebug("duscoScene: ERROR: unknown scene type");
           return;
        }
        addScene(newScene,addCmd.value("name").toString(),"",1.0);
    }
}

QString DiaScene::getRequestType()
{

    return "diaScene";
}

void DiaScene::stop()
{
    if(current != -1)
        scenes.at(current).data()->scene.data()->stop();
    if(fadingTo != -1)
        scenes.at(fadingTo).data()->scene.data()->stop();
}

void DiaScene::start()
{
    if(current != -1)
        scenes.at(current).data()->scene.data()->start();
    if(fadingTo != -1)
        scenes.at(fadingTo).data()->scene.data()->start();
    if(current == -1 && fadingTo == -1 && scenes.length() > 0){
        current = 0;
        scenes.at(current).data()->scene.data()->start();
    }
}

QJsonObject DiaScene::serialize()
{
    return serializeScene(QJsonObject());
}

QString DiaScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString DiaScene::getSceneTypeStringStaticaly()
{
    return "diaScene";
}

void DiaScene::addScene(QSharedPointer<Scene> scene,QString name,QString desc,float fadeInDuration)
{
    scenes.append(QSharedPointer<Dia>(new Dia(scene,name,desc,fadeInDuration,wss)));
    sendMsg(getState(true),false);
}

QJsonObject DiaScene::getState(bool addScenes)
{
    QJsonObject msg;
    if(addScenes){
        QJsonArray dias;
        for (int i = 0; i < scenes.length();i++) {
            QSharedPointer<Dia> dia = scenes.at(i);
            QJsonObject diaObj;
            diaObj.insert("id",i);
            diaObj.insert("requestId",dia.data()->providerId);
            diaObj.insert("name",dia.data()->name);
            dias.append(diaObj);
        }
        msg.insert("scenes",dias);
    }
    msg.insert("currentScene",fadingTo == -1 ? current : fadingTo);
    return msg;
}
