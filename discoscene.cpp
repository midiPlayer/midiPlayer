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
                    fusion.fusion(effect->scene,effect->fusionType,0.5f);
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
    if(msg.contains("fusionTypeChanged")){
        QJsonObject fusionTypeChange = msg.value("fusionTypeChanged").toObject();
        int sceneId = fusionTypeChange.value("sceneId").toInt(-1);
        QString fusionTypeStr = fusionTypeChange.value("fusionType").toString("");
        Device::FusionType fusionType;

        if(fusionTypeStr == "max")
            fusionType = Device::MAX;
        else if(fusionTypeStr == "min")
            fusionType = Device::MIN;
        else if(fusionTypeStr == "maxg")
            fusionType = Device::MAXG;
        else if(fusionTypeStr == "ming")
            fusionType = Device::MING;
        else if(fusionTypeStr == "av")
            fusionType = Device::AV;
        else if(fusionTypeStr == "override")
            fusionType = Device::OVERRIDE;
        DiscoSubScene *sub = effects.value(sceneId);
        sub->fusionType = fusionType;
    }
    sendMsgButNotTo(msg,id);
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
    effects.insert(sceneIdCounter,new DiscoSubScene{sceneIdCounter,scene,false,1.0,Device::MAX});
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

    QString fusionType;
    switch (effect->fusionType) {
    case Device::MAX:
        fusionType = "max";
        break;
    case Device::MAXG:
        fusionType = "maxg";
        break;
    case Device::MIN:
        fusionType = "min";
        break;
    case Device::MING:
        fusionType = "ming";
        break;
    case Device::AV:
        fusionType = "av";
        break;
    case Device::OVERRIDE:
        fusionType = "override";
        break;
    }
    effectObj.insert("fusionType",fusionType);

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

