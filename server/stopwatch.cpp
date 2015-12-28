#include "stopwatch.h"
#include "websocketserver.h"

Stopwatch::Stopwatch(WebSocketServer *ws) : time(getTimestamp()),running(false),snaped(0),WebSocketServerProvider(ws),timer()
{
    ws->registerProvider(this);
    timer.setInterval(1000);
    connect(&timer,SIGNAL(timeout()),this,SLOT(sendTimeSync()));
}

void Stopwatch::start(bool notify)
{
    time = getTimestamp();
    running = true;

    if(notify){
        QJsonObject ret;
        ret.insert("start",true);
        sendMsg(ret,true);
    }

    timer.start();
}

void Stopwatch::stop(bool notify)
{
    snaped = getMSecs();
    running = false;

    if(notify){
        QJsonObject ret;
        ret.insert("stop",true);
        sendMsg(ret,true);
    }

    timer.stop();
}

void Stopwatch::resume(bool notify)
{
    setTo(snaped);
    running = true;

    if(notify){
        QJsonObject ret;
        ret.insert("resume",true);
        sendMsg(ret,true);
    }
    timer.start();
}

int Stopwatch::getMSecs()
{
    if(running)
        return getTimestamp() - time;
    return snaped;
}

void Stopwatch::setTo(int ms, bool notify)
{
    snaped = ms;
    time = getTimestamp() - ms;
    if(notify)
        sendTimeSync();
}

void Stopwatch::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject ret;
    ret.insert("set",getMSecs());
    if(running)
        ret.insert("start",true);
    else
        ret.insert("stop",true);
    sendMsg(ret,id,true);
}

void Stopwatch::clientUnregistered(QJsonObject msg, int id)
{

}

void Stopwatch::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("start")){
        start(false);
        sendMsgButNotTo(msg,id,true);
    }
    if(msg.contains("stop")){
        stop(false);
        sendMsgButNotTo(msg,id,true);
    }
    if(msg.contains("resume")){
        resume(false);
        sendMsgButNotTo(msg,id,true);
    }
    if(msg.contains("set")){
        setTo(msg.value("set").toInt(),false);
        sendMsgButNotTo(msg,id,true);
    }
}

QString Stopwatch::getRequestType()
{
    return "stopwatch";
}

long Stopwatch::getTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}

void Stopwatch::sendTimeSync()
{
    QJsonObject ret;
    ret.insert("set",getMSecs());
    sendMsg(ret,true);
}
