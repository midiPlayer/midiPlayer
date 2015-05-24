#include "discosubscene.h"
#include "websocketserverprovider.h"

#define KEY_OPACITY "opacity"
#define KEY_MUTE "mute"
#define KEY_FUSION_TYPE "fusionType"
#define KEY_SCENE "scene"

DiscoSubScene::DiscoSubScene(int idP, QSharedPointer<Scene> sceneP, Device::FusionType fusionTypeP, bool muteP, float opacityP):
    id(idP),scene(sceneP),fusionType(fusionTypeP),mute(muteP),opacity(opacityP)
{

}

DiscoSubScene::DiscoSubScene(QJsonObject serialized, SceneBuilder *sceneBuilder, int idP): mute(true),id(idP)
{
    if(serialized.contains(KEY_MUTE)){
        mute = serialized.value(KEY_MUTE).toBool(true);
    }
    if(serialized.contains(KEY_OPACITY)){
        opacity = serialized.value(KEY_OPACITY).toDouble(1.0);
    }
    if(serialized.contains(KEY_FUSION_TYPE)){
        QString fusionTypeStr = serialized.value(KEY_FUSION_TYPE).toString("");
        setFusinType(fusionTypeStr);
    }
    if(serialized.contains(KEY_SCENE)){
        scene = sceneBuilder->build(serialized.value(KEY_SCENE).toObject());
    }
}

QJsonObject DiscoSubScene::getJsonForClient()
{
    QJsonObject effectObj = getBasicJson();

    WebSocketServerProvider *provider = dynamic_cast<WebSocketServerProvider*>(scene.data());
    if(provider != 0){
        effectObj.insert("requestType",provider->getRequestType());
        effectObj.insert("providerId",provider->providerId);
    }
    effectObj.insert("name",scene.data()->getName());
    return effectObj;
}

QJsonObject DiscoSubScene::getBasicJson()
{
    QJsonObject effectObj;
    effectObj.insert("sceneId",id);
    effectObj.insert(KEY_MUTE,mute);
    effectObj.insert(KEY_OPACITY,opacity);

    QString fusionTypeStr;
    switch (fusionType) {
    case Device::MAX:
        fusionTypeStr = "max";
        break;
    case Device::MAXG:
        fusionTypeStr = "maxg";
        break;
    case Device::MIN:
        fusionTypeStr = "min";
        break;
    case Device::MING:
        fusionTypeStr = "ming";
        break;
    case Device::AV:
        fusionTypeStr = "av";
        break;
    case Device::OVERRIDE:
        fusionTypeStr = "override";
        break;
    }
    effectObj.insert("fusionType",fusionTypeStr);

    return effectObj;
}

QJsonObject DiscoSubScene::serialize(SceneBuilder *builder)
{
 QJsonObject effectObj = getBasicJson();
 effectObj.insert(KEY_SCENE,builder->serializeScene(scene.data(),scene.data()->serialize()));
 return effectObj;
}

void DiscoSubScene::setFusinType(QString fusionTypeStr)
{
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
}

