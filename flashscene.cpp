#include "flashscene.h"
#include "websocketserver.h"

void FlashScene::init(WebSocketServer *ws)
{
    ws->registerProvider(this);
    trigger.triggerConfig.insert(Trigger::BEAT);
    connect(&trigger,SIGNAL(trigger()),this,SLOT(triggered()));
    foreach (Device dev, availableDevices) {
        foreach (int c, dev.getChannels()) {
            dev.setChannel(c,254.0f);
        }
        flashState.append(dev);
    }
}

FlashScene::FlashScene(QString name, WebSocketServer *ws,QList<Device> avDevP,JackProcessor *jackP) : Scene(name),
    WebSocketServerProvider(ws),trigger(ws,jackP),availableDevices(avDevP),flashEnabled(false),flashState(),time(),smoothDuration(200),beatSpeed(INT_MAX),timePer(0.0f)
{
    init(ws);
}

FlashScene::FlashScene(QJsonObject serialized,WebSocketServer *ws,QList<Device> avDevP,JackProcessor *jackP) : Scene(serialized),
        WebSocketServerProvider(ws),trigger(ws,jackP),availableDevices(avDevP),
        flashEnabled(false),flashState(),time(),smoothDuration(200),beatSpeed(INT_MAX),timePer(0.0f)
{

}


QList<Device> FlashScene::getLights()
{

        int realSmoothDuration = smoothDuration;
        if(beatSpeed < realSmoothDuration)
            realSmoothDuration = beatSpeed;//optherwise the animation will get cut


        if(time.elapsed() > smoothDuration){
            flashEnabled = false;
        }

        // mit weiß beim beat:
        float timePer = 0;
        if(time.elapsed() < smoothDuration){
            timePer = 1.0f - (float(time.elapsed()) / float(smoothDuration));
            if(timePer > 1)
                timePer = 1;
        }
        else if(time.elapsed() > beatSpeed-realSmoothDuration){
            timePer = float(time.elapsed()-(beatSpeed-realSmoothDuration))/float(realSmoothDuration);
            if(timePer > 1)
                timePer = 1-timePer;
        }

/*
        //start width beat
        timePer = float(time.elapsed()) / float(realSmoothDuration);
        timePer -= 0.5;
        if(timePer<0.0f) timePer *= -1;
        timePer *= 2;
        timePer = 1-timePer;

*/



        if(timePer < 0)
            timePer = 0;

        QList<Device> ret;
        foreach (Device d, flashState) {
            ret.append(d*timePer);
        }

        return ret;

}

QList<Device> FlashScene::getUsedLights()
{
    if(flashEnabled)
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

QJsonObject FlashScene::serialize(QJsonObject ret){
    ret.insert("trigger",trigger.serialize());
    ret.insert("smoothDuration",smoothDuration);
    return Scene::serialize(ret);
}

void FlashScene::triggered()
{
    flashEnabled = 1;
    beatSpeed = time.elapsed();
    time.restart();
}
