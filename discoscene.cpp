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
        foreach (int orderId, order) {
            if(!effects.contains(orderId))
                continue;
            DiscoSubScene *effect = effects.value(orderId);
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
    QJsonObject response = getStatus(true,true,false);
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
    if(msg.contains("orderChanged")){
        QJsonArray readOrder = msg.value("orderChanged").toArray();
        QList<int> newOrder;
        foreach (QJsonValue val, readOrder) {
            int i = val.toInt(-2);
            if(i != -2 && effects.contains(i))
                newOrder.append(i);
        }
        order.clear();
        order = newOrder;
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

QJsonObject DiscoScene::serialize()
{
    QJsonObject ret = getStatus(true,true,true);
    return Scene::serialize(ret);
}


void DiscoScene::addEffect(Scene *scene)
{
    effects.insert(sceneIdCounter,new DiscoSubScene(sceneIdCounter,scene,Device::MAX,false,1.0));
    order.append(sceneIdCounter);
    sceneIdCounter++;
}

/**
 * @brief DiscoScene::getStatus
 * @param showEffects
 * @param showOrder
 * @param serialize not generate client json bt generate serialized json width sub json.
 * @return void
 */
QJsonObject DiscoScene::getStatus(bool showEffects,bool showOrder,bool serialize)
{
    QJsonObject status;
    if(showEffects){
        QJsonArray effectsObj;
        foreach(DiscoSubScene *effect,effects){
            if(serialize)
                effectsObj.append(effect->serialize());
            else
                effectsObj.append(effect->getJsonForClient());
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

