#include "discoscene.h"
#include "fusionscene.h"
#include "websocketserver.h"
#include <QJsonArray>


#define STATUS_KEY_EFFECTS "effects"

DiscoScene::DiscoScene(WebSocketServer *ws, SceneBuilder *sceneBuilderP, QString name, QJsonObject serialized) : Scene(name,serialized),
    WebSocketServerProvider(ws), fusion("fusion"),solo(false),soloScene(), sceneIdCounter(0),order(),sceneBuilder(sceneBuilderP),isRunning(false)
{
    ws->registerProvider(this);

    if(serialized.length() != 0 && serialized.contains(STATUS_KEY_EFFECTS) && serialized.value(STATUS_KEY_EFFECTS).isArray()){
        foreach (QJsonValue effectVal, serialized.value(STATUS_KEY_EFFECTS).toArray()) {
            QJsonObject effectJson = effectVal.toObject();
            QSharedPointer<DiscoSubScene> scene = QSharedPointer<DiscoSubScene>(
                        new DiscoSubScene(effectJson,sceneBuilder,sceneIdCounter));
            if(!(scene.isNull() || scene.data()->scene.isNull()))
                addSubScene(scene);
        }
    }
}

QMap<QString, QSharedPointer<DeviceState> > DiscoScene::getDeviceState()
{
    fusion.reset();
    //fusion.import(getUsedLights());//all light in off state
    if(solo){
        fusion.import(soloScene.data()->scene.data()->getDeviceState());
    }
    else{
        foreach  (int orderId, order) {
            if(!effects.contains(orderId))
                continue;
            QSharedPointer<DiscoSubScene> effect = effects.value(orderId);
            if(!effect.data()->mute)
                fusion.fusion(effect.data()->scene,effect.data()->fusionType,0.5f);
        }
    }
    return fusion.getDeviceState();
}


QString DiscoScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString DiscoScene::getSceneTypeStringStaticaly()
{
    return "discoScene";
}

void DiscoScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject response = getStatus(true,true,false);
    sendMsg(response,id,true);
}

void DiscoScene::clientUnregistered(QJsonObject msg, int id)
{

}

void DiscoScene::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("muteChanged")){
        QJsonObject muteChange = msg.value("muteChanged").toObject();
        int sceneId = muteChange.value("sceneId").toInt(-1);
        if(effects.contains(sceneId)){
            bool muteState = muteChange.value("state").toBool(false);
            DiscoSubScene *sub = effects.value(sceneId).data();
            if(isRunning && !(solo && soloScene.data() == sub)){
                if(muteState)
                    sub->scene.data()->stop();
                else if(!muteState && sub->mute)
                    sub->scene.data()->start();
            }
            sub->mute = muteState;

        }
        sendMsgButNotTo(msg,id,true);
    }
    if(msg.contains("soloChanged")){
        QJsonObject soloChange = msg.value("soloChanged").toObject();
        if(soloChange.value("state").toBool()){
            int sceneId = soloChange.value("sceneId").toInt(-1);
            if(effects.contains(sceneId)){
                if(effects.value(sceneId).data()->mute && (soloScene != effects.value(sceneId) || !solo) && isRunning)
                    effects.value(sceneId).data()->scene.data()->start();
                solo = true;
                soloScene = effects.value(sceneId);
            }
        }
        else{
            if(soloScene.data()->mute && isRunning)
                soloScene.data()->scene.data()->stop();
            solo = false;
        }
        sendMsgButNotTo(msg,id,true);
    }
    if(msg.contains("fusionTypeChanged")){
        QJsonObject fusionTypeChange = msg.value("fusionTypeChanged").toObject();
        int sceneId = fusionTypeChange.value("sceneId").toInt(-1);
        if(effects.contains(sceneId)){
            QString fusionTypeStr = fusionTypeChange.value("fusionType").toString("");
            DiscoSubScene *sub = effects.value(sceneId).data();
            sub->setFusinType(fusionTypeStr);
        }
        sendMsgButNotTo(msg,id,true);
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
        sendMsgButNotTo(msg,id,true);
    }
    if(msg.contains("addScene")){
        QJsonObject addCmd = msg.value("addScene").toObject();
        QSharedPointer<Scene> newScene = sceneBuilder->build(addCmd.value("type").toString(),addCmd.value("name").toString());
        if(newScene.isNull()){
           qDebug("duscoScene: ERROR: unknown scene type");
           return;
        }
        addSubScene(QSharedPointer<DiscoSubScene>(new DiscoSubScene(sceneIdCounter,newScene,ChannelDeviceState::MAX,true,1.0f)));
        sendMsg(getStatus(true,true,false),id,true);
    }
    if(msg.contains("deleteScene")){
        int id = msg.value("deleteScene").toInt(-1);
        if(id != -1){
            order.removeAll(id);
            effects.remove(id);
        }
        sendMsgButNotTo(msg,id,true);
    }
}

QString DiscoScene::getRequestType()
{
    return "discoScene";
}

void DiscoScene::stop()
{
    isRunning = false;
    foreach (QSharedPointer<DiscoSubScene> dss, effects.values()) {
        dss.data()->scene.data()->stop();
    }
}

void DiscoScene::start()
{
    solo = false;
    isRunning = true;
    foreach (QSharedPointer<DiscoSubScene> dss, effects.values()) {
        if(dss.data()->mute)
            continue;
        dss.data()->scene.data()->start();
    }
}

QJsonObject DiscoScene::serialize()
{
    QJsonObject ret = getStatus(true,false,true);
    return serializeScene(ret);
}


void DiscoScene::addEffect(QSharedPointer<Scene> scene)
{
    addSubScene(QSharedPointer<DiscoSubScene>(new DiscoSubScene(sceneIdCounter,scene,ChannelDeviceState::MAX,false,1.0)));
}

void DiscoScene::addSubScene(QSharedPointer<DiscoSubScene> subScene){
    effects.insert(sceneIdCounter,subScene);
    order.append(sceneIdCounter);
    sceneIdCounter++;
    if(isRunning)
        subScene.data()->scene.data()->start();
    else
        subScene.data()->scene.data()->stop();
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
        foreach(int id,order){
            QSharedPointer<DiscoSubScene> effect = effects.value(id);
            if(serialize)
                effectsObj.append(effect.data()->serialize(sceneBuilder));
            else
                effectsObj.append(effect.data()->getJsonForClient());
        }
        status.insert(STATUS_KEY_EFFECTS,effectsObj);
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

