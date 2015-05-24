#include "discosubscene.h"
#include "websocketserverprovider.h"

DiscoSubScene::DiscoSubScene(int idP, QSharedPointer<Scene> sceneP, Device::FusionType fusionTypeP, bool muteP, float opacityP):
    id(idP),scene(sceneP),fusionType(fusionTypeP),mute(muteP),opacity(opacityP)
{

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
    effectObj.insert("mute",mute);
    effectObj.insert("opacity",opacity);

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

QJsonObject DiscoSubScene::serialize()
{
 QJsonObject effectObj = getBasicJson();
 effectObj.insert("scene",scene.data()->serialize());
 return Serializable::serialize(effectObj);
}

