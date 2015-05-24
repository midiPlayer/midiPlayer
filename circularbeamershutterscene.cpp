#include "circularbeamershutterscene.h"
#include "websocketserver.h"

CircularBeamerShutterScene::CircularBeamerShutterScene(QString nameP , beamerDeviceProvider *providerP,
                                                       WebSocketServer *ws, JackProcessor *p) :
    QObject(0),BeamerShutterScene(nameP,ws), provider(providerP),trigger(ws,p),isRunning(false),x(0.5),y(0.5),radius(0.5),oldPos(-1)
{
    trigger.triggerConfig.insert(Trigger::BEAT);
    connect(&trigger,SIGNAL(trigger()),this,SLOT(triggered()));
    ws->registerProvider(this);
}

void CircularBeamerShutterScene::start()
{
    trigger.start();
    isRunning = true;
}

void CircularBeamerShutterScene::stop()
{
    trigger.stop();
    isRunning = false;
}

void CircularBeamerShutterScene::clientRegistered(QJsonObject msg, int clientIdCounter)
{

}

void CircularBeamerShutterScene::clientUnregistered(QJsonObject msg, int clientIdCounter)
{

}

void CircularBeamerShutterScene::clientMessage(QJsonObject msg, int clientId)
{
    if(msg.contains("radiusChanged")){
        radius = msg.value("radiusChanged").toDouble();
    }
    if(msg.contains("centerChanged")){
        QJsonObject change = msg.value("centerChanged").toObject();
        if(change.contains("x"))
            x = change.value("x").toDouble(0);
        if(change.contains("y"))
            y = change.value("y").toDouble(0);
    }
    dataChanged();
    sendMsgButNotTo(msg,clientId,true);
}

QString CircularBeamerShutterScene::getRequestType()
{
    return "circualrBeamerShutter";
}

void CircularBeamerShutterScene::dataChanged()
{
    if(!isRunning)
        return;//should never happen
    QJsonObject params;
    params.insert("radius",radius);
    params.insert("x",x);
    params.insert("y",y);
    provider->publishShutter(BeamerDevice(BeamerDevice::CIRCULAR,params),true);//send To all beamers
}

void CircularBeamerShutterScene::triggered()
{
    int pos;
    do
        pos = rand()%5;
    while(pos == oldPos);
    oldPos = pos;
    switch(pos){
        case 0: x=0.5; y=0.5; break;
        case 1: x=0; y=0; break;
        case 2: x=0; y=1; break;
        case 3: x=1; y=0; break;
        case 4: x=1; y=1; break;
        default: x=0.5; y=0.5; break;
    }
    dataChanged();
}

