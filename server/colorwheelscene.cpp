#include "colorwheelscene.h"
#include "websocketserver.h"
#include <QColor>
#include <algorithm>
#include "math.h"
#include "channeldevice.h"

#define KEY_TRIGGER "trigger"
#define KEY_DURATION "duration"
#define KEY_BASIC_SPEED "basicSpeed"
#define KEY_BUMP_SPEED "bumpSpeed"

ColorWheelScene::ColorWheelScene(VirtualDeviceManager *manager, WebSocketServer *ws, JackProcessor *jackP, QString name, QJsonObject serialized) :
    Scene(name,serialized),WebSocketServerProvider(ws),filterDevManager(manager), stopwatch(),triggerStopwatch(),positionedDevices(),basicSpeed(0.05),anchor(0.0f),
    trigger(ws,jackP),duration(0.6),bumpSpeed(0.5)
{
    ws->registerProvider(this);
    trigger.triggerConfig.insert(Trigger::BEAT);
    connect(&trigger,SIGNAL(trigger()),(ColorWheelScene*)this,SLOT(triggered()));
    stopwatch.start();
    triggerStopwatch.start();

    filterDevManager.addAcceptedType(Device::RGB);
    filterDevManager.addAcceptedType(Device::RGBW);
    filterDevManager.addAcceptedType(Device::Beamer);
    connect(&filterDevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(updateDevices()));
    updateDevices();



    if(serialized.length() > 0){
        if(serialized.contains(KEY_TRIGGER))
            trigger.loadSerialized(serialized.value(KEY_TRIGGER).toObject());
        if(serialized.contains(KEY_BASIC_SPEED))
            basicSpeed = serialized.value(KEY_BASIC_SPEED).toDouble(0);
        if(serialized.contains(KEY_BUMP_SPEED))
            bumpSpeed = serialized.value(KEY_BUMP_SPEED).toDouble(0);
        if(serialized.contains(KEY_DURATION))
            duration = serialized.value(KEY_DURATION).toDouble(1.0);
    }
}

QMap<QString, QSharedPointer<DeviceState> > ColorWheelScene::getDeviceState()
{
    anchor += getDelta();
    while(anchor>1.0)
        anchor-=1.0;

    QMap<QString, QSharedPointer<DeviceState> > ret;
    foreach(PositionedDevice posDev, positionedDevices){
        float individualAnchor = anchor+(posDev.angle/360);
        while(individualAnchor > 1.0)
            individualAnchor -= 1.0;
        QSharedPointer<ChannelDeviceState> state = posDev.d.data()->createEmptyState().dynamicCast<ChannelDeviceState>();
        QColor color = normalizeColor(QColor::fromHsvF(individualAnchor,1,1));
        int lowestChannel = state.data()->getFirstChannel();

        state.data()->setChannel(lowestChannel + 0,color.redF());
        state.data()->setChannel(lowestChannel + 1,color.greenF());
        state.data()->setChannel(lowestChannel + 2,color.blueF());
        if(posDev.d.data()->getType() == Device::RGBW)
            state.data()->setChannel(lowestChannel + 3,0.0);

        ret.insert(posDev.d.data()->getDeviceId(),state);
    }

    stopwatch.restart();

    return ret;
}

void ColorWheelScene::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject reply;
    reply.insert("durationChanged",duration);
    reply.insert("basicSpeedChanged",basicSpeed);
    reply.insert("bumpSpeedChanged",bumpSpeed);
    reply.insert("trigger",trigger.providerId);
    sendMsg(reply,id,true);
}

void ColorWheelScene::clientUnregistered(QJsonObject msg, int id)
{

}

void ColorWheelScene::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("bumpSpeedChanged"))
        bumpSpeed = msg.value("bumpSpeedChanged").toDouble(0);
    if(msg.contains("basicSpeedChanged"))
        basicSpeed = msg.value("basicSpeedChanged").toDouble(0);
    if(msg.contains("durationChanged"))
        duration = msg.value("durationChanged").toDouble(1.0);
    sendMsgButNotTo(msg,id,true);
}

QString ColorWheelScene::getRequestType()
{
    return "colorWheel";
}

void ColorWheelScene::stop()
{
    trigger.stop();
}

void ColorWheelScene::start()
{
    trigger.start();
}

QJsonObject ColorWheelScene::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_TRIGGER,trigger.providerId);
    ret.insert(KEY_BASIC_SPEED,basicSpeed);
    ret.insert(KEY_BUMP_SPEED,bumpSpeed);
    ret.insert(KEY_DURATION,duration);
    return serializeScene(ret);
}

QString ColorWheelScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString ColorWheelScene::getSceneTypeStringStaticaly()
{
    return "colorWheel";
}

void ColorWheelScene::triggered()
{
    triggerStopwatch.restart();
    //qDebug() << "triggered";
}

void ColorWheelScene::updateDevices()
{
    QVector3D middle = QVector3D(0,0,0);
    int  div = 0;
    foreach (QSharedPointer<Device> d, filterDevManager.getDevices().values()) {
        middle += d.data()->getPosition();
        div++;
    }
    if(div != 0){
        middle /= div;
        qDebug() << "middle:" << middle;
        QVector3D reference(0,0,0);
        QVector3D normal(0,0,0);
        middle.setZ(0);//2D
        foreach (QSharedPointer<Device> d, filterDevManager.getDevices().values()) {
            QVector3D devPos = d.data()->getPosition();
            devPos.setZ(0);//2D
            QVector3D toDev = devPos - middle;
            float angle = 0.0f;
            if(reference.length() == 0){//make this device as reference
                reference = toDev;
                angle = 0.0f;
                normal = QVector3D::crossProduct(reference,QVector3D(0,0,1));
            }
            else{
                double p = QVector3D::dotProduct(toDev,reference)/(reference.length()*toDev.length());
                angle = acos(p) / M_PI * 180.0;
                if(QVector3D::dotProduct(toDev,normal)<0){
                    angle = 360-angle;
                }
                qDebug() << "value:" << p << " a:" << angle;
            }

            PositionedDevice posD = {d,angle};
            positionedDevices.append(posD);
        }
    }
}

/**
 * @brief ColorWheelScene::getSpeed
 * @param time in seconds
 * @return
 */
double ColorWheelScene::getSpeed(double time)
{

    double m = bumpSpeed / duration;
    return std::max((basicSpeed+bumpSpeed) - m*time,double(basicSpeed));
}

double ColorWheelScene::getDelta()
{
    double delta = stopwatch.elapsed()/1000.0f;
    double now = triggerStopwatch.elapsed()/1000.0f;
    double prev = now - delta;
    double avSpeed = (getSpeed(now));
    //qDebug() << "avSpeed:" << avSpeed;
    return delta*avSpeed;
}

QColor ColorWheelScene::normalizeColor(QColor color)
{
    return color;

}

