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
            QSharedPointer<DiscoSubScene> effect = effects.value(orderId);
            if(!effect.data()->mute)
                fusion.fusion(effect.data()->scene,effect.data()->fusionType,0.5f);
        }
    }
    return fusion.getLights();
}

QList<Device> DiscoScene::getUsedLights()
{
    QList<Device> useedLights;
    foreach (QSharedPointer<DiscoSubScene> effect, effects.values()) {
        if(effect.data()->mute)
            continue; //TODO add handling for unused devices
        foreach(Device d,effect->scene.data()->getUsedLights()){
            if(useedLights.contains(d))
                useedLights.append(d);
        }
    }

    return useedLights;
}

QString DiscoScene::getSceneTypeString()
{
    return "discoScene";
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
        DiscoSubScene *sub = effects.value(sceneId).data();
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
        DiscoSubScene *sub = effects.value(sceneId).data();
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
    foreach (QSharedPointer<DiscoSubScene> dss, effects.values()) {
        dss.data()->scene.data()->stop();
    }
}

void DiscoScene::start()
{
    foreach (QSharedPointer<DiscoSubScene> dss, effects.values()) {
        dss.data()->scene.data()->start();
    }
}

QJsonObject DiscoScene::serialize()
{
    QJsonObject ret = getStatus(true,true,true);
    return Scene::serialize(ret);
}


void DiscoScene::addEffect(QSharedPointer<Scene> scene)
{
    effects.insert(sceneIdCounter,QSharedPointer<DiscoSubScene>(new DiscoSubScene(sceneIdCounter,scene,Device::MAX,false,1.0)));
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
        foreach(QSharedPointer<DiscoSubScene> effect,effects){
            if(serialize)
                effectsObj.append(effect.data()->serialize());
            else
                effectsObj.append(effect.data()->getJsonForClient());
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

