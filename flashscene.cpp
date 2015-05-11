#include "flashscene.h"
#include "websocketserver.h"

FlashScene::FlashScene(QString name, WebSocketServer *ws,QList<Device> avDevP,JackProcessor *jackP) : Scene(name),
    WebSocketServerProvider(ws),trigger(ws,jackP),avDev(avDevP),flash(false),flashState(),time()
{
    ws->registerProvider(this);
    trigger.triggerConfig.insert(Trigger::BEAT);
    connect(&trigger,SIGNAL(trigger()),this,SLOT(triggered()));
    foreach (Device dev, avDev) {
        foreach (int c, dev.getChannels()) {
            dev.setChannel(c,254.0f);
        }
        flashState.append(dev);
    }
}

QList<Device> FlashScene::getLights()
{
    if(flash){
        if(time.elapsed() > 100){
            flash = false;
        }
        return flashState;
    }
    else
        return QList<Device>();
}

QList<Device> FlashScene::getUsedLights()
{
    if(flash)
        return flashState;
    else
        return QList<Device>();
}



void FlashScene::start()
{
    trigger.start();
}

void FlashScene::stop()
{
    trigger.stop();
}

void FlashScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject replay;
    QJsonObject config;
    config.insert("trigger",trigger.providerId);
    replay.insert("config",config);
    sendMsg(replay,id,true);
}

void FlashScene::clientUnregistered(QJsonObject msg, int clientIdCounter)
{

}

void FlashScene::clientMessage(QJsonObject msg, int clientIdCounter)
{

}

QString FlashScene::getRequestType()
{
    return "falshScene";
}

void FlashScene::triggered()
{
    flash = 1;
    time.restart();
}
