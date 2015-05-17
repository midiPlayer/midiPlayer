#include "circularbeamershutterscene.h"

CircularBeamerShutterScene::CircularBeamerShutterScene(beamerDeviceProvider *providerP,
                                                       WebSocketServer *ws, JackProcessor *p) :
    QObject(0),BeamerShutterScene(), provider(providerP),trigger(ws,p),isRunning(false),x(0.5),y(0.5),radius(0.5),oldPos(-1)
{
    trigger.triggerConfig.insert(Trigger::BEAT);
    connect(&trigger,SIGNAL(trigger()),this,SLOT(triggered()));
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

void CircularBeamerShutterScene::triggered()
{
    int pos;
    while(pos == oldPos)
        pos = rand()%5;
    oldPos = pos;
    switch(pos){
        case 0: x=0.5; y=0.5; break;
        case 1: x=0; y=0; break;
        case 2: x=0; y=1; break;
        case 3: x=1; y=0; break;
        case 4: x=1; y=1; break;
        default: x=0.5; y=0.5; break;
    }

    if(!isRunning)
        return;//should never happen
    QJsonObject params;
    params.insert("radius",radius);
    params.insert("x",x);
    params.insert("y",y);
    provider->publishShutter(BeamerDevice(BeamerDevice::CIRCULAR,params),true);//send To all beamers
}

