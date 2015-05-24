#include "scenebuilder.h"
#include "flashscene.h"
#include "discoscene.h"
#include "beatscene1.h"
#include "colorscene.h"
SceneBuilder::SceneBuilder(WebSocketServer *wssP, QList<Device> *avDev, JackProcessor *jackP):wss(wssP),availableDevices(avDev),
    jack(jackP)
{

}

QSharedPointer<Scene> SceneBuilder::build(QString sceneType, QString name,QJsonObject serialized)
{
    if(sceneType == FlashScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new FlashScene(wss,*availableDevices,jack,name,serialized));
    }
    else if(sceneType == DiscoScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new DiscoScene(wss,this,name,serialized));
    }
    else if(sceneType == BeatScene1::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new BeatScene1(*availableDevices,jack,wss,name,serialized));
    }
    else if(sceneType == ColorScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new ColorScene(*availableDevices,name,serialized));
    }
}

QSharedPointer<Scene> SceneBuilder::build(QJsonObject serialized)
{
    return build(serialized.value("sceneType").toString(),"",serialized.value("sceneParams").toObject());
}

QJsonObject SceneBuilder::serializeScene(QSharedPointer<Scene> scene)
{
    return serializeScene(scene.data());
}

QJsonObject SceneBuilder::serializeScene(Scene *scene)
{
    return serializeScene(scene,scene->serialize());
}

QJsonObject SceneBuilder::serializeScene(Scene *scene, QJsonObject serialized)
{
    QJsonObject ret;
    ret.insert("sceneType",scene->getSceneTypeString());
    ret.insert("sceneParams",serialized);
    return ret;
}

