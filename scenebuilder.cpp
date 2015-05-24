#include "scenebuilder.h"
#include "flashscene.h"
SceneBuilder::SceneBuilder(WebSocketServer *wssP, QList<Device> avDev, JackProcessor *jackP):wss(wssP),availableDevices(avDev),
    jack(jackP)
{

}

QSharedPointer<Scene> SceneBuilder::build(QString sceneType, QString name)
{
    if(sceneType == FlashScene::getSceneTypeStringStaticaly()){
        QSharedPointer<Scene>(new FlashScene(name,wss,availableDevices,jack));
    }
}

QSharedPointer<Scene> SceneBuilder::build(QString sceneType, QJsonObject serialied)
{
}

