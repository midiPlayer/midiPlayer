#include "diascene.h"
#include "websocketserver.h"
DiaScene::DiaScene(QList<Device> avDev, WebSocketServer *ws,
                   JackProcessor *jackP, QString name, QJsonObject serialized):
                Scene(name,serialized),WebSocketServerProvider(ws),availableDevices(avDev),
                current(-1),fadingTo(-1),fadeTimer(),fusion("fusion")
{
    ws->registerProvider(this);
}

QList<Device> DiaScene::getLights()
{
    fusion.reset();

    if(current != -1)
        fusion.import(scenes.at(current).scene.data());
    if(fadingTo != -1){
        Dia fadingToDia  =scenes.at(fadingTo);
        float percentage = fadeTimer.elapsed()/fadingToDia.fadeInDuration;
        if(percentage> 1){
            percentage = 1;
            current = fadingTo;
        }
        fusion.fusion(fadingToDia.scene,Device::AV,percentage);
    }

    return fusion.getLights();
}

QList<Device> DiaScene::getUsedLights()
{
    return availableDevices;
}

void DiaScene::clientRegistered(QJsonObject msg, int id)
{

}

void DiaScene::clientUnregistered(QJsonObject msg, int id)
{

}

void DiaScene::clientMessage(QJsonObject msg, int id)
{

}

QString DiaScene::getRequestType()
{

    return "diaScene";
}

void DiaScene::stop()
{
    if(current != -1)
        scenes.at(current).scene.data()->stop();
    if(fadingTo != -1)
        scenes.at(fadingTo).scene.data()->stop();
}

void DiaScene::start()
{
    if(current != -1)
        scenes.at(current).scene.data()->start();
    if(fadingTo != -1)
        scenes.at(fadingTo).scene.data()->start();
    if(current == -1 && fadingTo == -1 && scenes.length() > 0){
        current = 0;
        scenes.at(current).scene.data()->start();
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

void DiaScene::addScene(QSharedPointer<Scene> scene,float fadeInDuration)
{
    scenes.append({scene,fadeInDuration});
}
