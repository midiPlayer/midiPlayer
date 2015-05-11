#include "discoscene.h"
#include "fusionscene.h"
#include "websocketserver.h"
#include <QJsonArray>

DiscoScene::DiscoScene(QString name, WebSocketServer *ws) : Scene(name),WebSocketServerProvider(ws), fusion("fusion"),solo(false),sceneIdCounter(0),
    order()
{
    ws->registerProvider(this);
}



QList<Device> DiscoScene::getLights()
{
    fusion.reset();
    fusion.import(getUsedLights());//all light in off state
    if(solo){
        fusion.fusion(soloScene->getLights(),Device::OVERRIDE,1.0);
    }
    else{
        foreach (DiscoSubScene *effect, effects.values()) {
                if(!effect->mute)
                    fusion.fusion(effect->scene,Device::MAX,0.5f);
        }
    }
    return fusion.getLights();
}

QList<Device> DiscoScene::getUsedLights()
{
    QList<Device> useedLights;
    foreach (DiscoSubScene *effect, effects.values()) {
        if(effect->mute)
            continue; //TODO add handling for unused devices
        foreach(Device d,effect->scene->getUsedLights()){
            if(useedLights.contains(d))
                useedLights.append(d);
        }
    }

    return useedLights;
}

void DiscoScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject response = getStatus(true,true);
    sendMsg(response,id);
}

void DiscoScene::clientUnregistered(QJsonObject msg, int id)
{

}

void DiscoScene::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("muteChanged")){
        QJsonObject muteChange = msg.value("muteChanged").toObject();
        int sceneId = muteChange.value("sceneId").toInt(-1);
        bool muteState = muteChange.value("state").toBool(false);
        DiscoSubScene *sub = effects.value(sceneId);
        sub->mute = muteState;
    }
}

QString DiscoScene::getRequestType()
{
    return "discoScene";
}

void DiscoScene::stop()
{
    foreach (DiscoSubScene *dss, effects) {
        dss->scene->stop();
    }
}

void DiscoScene::start()
{
    foreach (DiscoSubScene *dss, effects) {
        dss->scene->start();
    }
}


void DiscoScene::addEffect(Scene *scene)
{
    effects.insert(sceneIdCounter,new DiscoSubScene{sceneIdCounter,scene,false,1.0});
    order.append(sceneIdCounter);
    sceneIdCounter++;
}

QJsonObject DiscoScene::getEffectJson(DiscoScene::DiscoSubScene *effect)
{
    QJsonObject effectObj;
    effectObj.insert("sceneId",effect->id);
    effectObj.insert("mute",effect->mute);
    effectObj.insert("opacity",effect->opacity);
    effectObj.insert("name",effect->scene->getName());
    WebSocketServerProvider *provider = dynamic_cast<WebSocketServerProvider*>(effect->scene);
    if(provider != 0){
        effectObj.insert("requestType",provider->getRequestType());
        effectObj.insert("providerId",provider->providerId);
    }
    return effectObj;
}

QJsonObject DiscoScene::getStatus(bool showEffects,bool showOrder)
{
    QJsonObject status;
    if(showEffects){
        QJsonArray effectsObj;
        foreach(DiscoSubScene *effect,effects){
            effectsObj.append(getEffectJson(effect));
        }
        status.insert("effects",effectsObj);
    }
    if(showOrder && order.size() > 0){
        QJsonArray orderObj;
        foreach(int id, order){
            orderObj.append(id);
        }
        status.insert("order",orderObj);
    }

    return status;
}

