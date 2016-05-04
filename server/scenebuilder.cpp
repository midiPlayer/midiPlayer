#include "scenebuilder.h"
#include "flashscene.h"
#include "discoscene.h"
#include "beatscene1.h"
#include "colorscene.h"
#include "colorwheelscene.h"
#include "colorwavescene.h"
#include "keyframescene.h"
#include "musicscene.h"
#include "screencolorscene.h"

SceneBuilder::SceneBuilder(WebSocketServer *wssP, VirtualDeviceManager *manager, JackProcessor *jackP):wss(wssP),
    vDevManager(manager),jack(jackP)
{

}

QSharedPointer<Scene> SceneBuilder::build(QString sceneType, QString name,QJsonObject serialized)
{
    if(sceneType == FlashScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new FlashScene(wss,jack,vDevManager,name,serialized));
    }
    else if(sceneType == DiscoScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new DiscoScene(wss,this,name,serialized));
    }
    else if(sceneType == BeatScene1::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new BeatScene1(vDevManager,jack,wss,name,serialized));
    }
    else if(sceneType == ColorScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new ColorScene(vDevManager,wss,name,serialized));
    }
    else if(sceneType == ColorWheelScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new ColorWheelScene(vDevManager,wss,jack,name,serialized));
    }
    else if(sceneType == ColorWaveScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new ColorWaveScene(vDevManager,wss,jack,name,serialized));
    }
    else if(sceneType == KeyFrameScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new KeyFrameScene(vDevManager,name,wss,serialized));
    }
    else if(sceneType == MusicScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new MusicScene(name,wss,serialized));
    }
    else if(sceneType == ScreenColorScene::getSceneTypeStringStaticaly()){
        return QSharedPointer<Scene>(new ScreenColorScene(vDevManager,wss,name,serialized));
    }
    return QSharedPointer<Scene>();
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

