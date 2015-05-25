#include "colorwheelscene.h"
#include "websocketserver.h"
#include <QColor>
#include <algorithm>
#include "math.h"

ColorWheelScene::ColorWheelScene(QList<Device> avDev, WebSocketServer *ws, JackProcessor *jackP, QString name, QJsonObject serialized) :
    Scene(name,serialized),WebSocketServerProvider(ws), stopwatch(),triggerStopwatch(),positionedDevices(),basicSpeed(0.05),anchor(0.0f),
    trigger(ws,jackP),duration(0.6),maxSpeed(0.5),usedDevices()
{
    ws->registerProvider(this);
    trigger.triggerConfig.insert(Trigger::BEAT);
    connect(&trigger,SIGNAL(trigger()),(ColorWheelScene*)this,SLOT(triggered()));
    stopwatch.start();
    triggerStopwatch.start();
    foreach (Device dev, avDev) {
        if(dev.getType() == Device::RGB || dev.getType() == Device::RGBW)
        usedDevices.append(dev);
    }

    QVector3D middle = QVector3D(0,0,0);
    int  div = 0;
    foreach (Device d, usedDevices) {
        middle += d.getPosition();
        div++;
    }
    if(div != 0){
        middle /= div;
        qDebug() << "middle:" << middle;
        QVector3D reference(0,0,0);
        QVector3D normal(0,0,0);
        middle.setZ(0);//2D
        foreach (Device d, usedDevices) {
            QVector3D devPos = d.getPosition();
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
    else{
        foreach (Device d, usedDevices) {
            PositionedDevice posD = {d,0.0f};
            positionedDevices.append(posD);
        }
    }
}

QList<Device> ColorWheelScene::getLights()
{

    anchor += getDelta();
    while(anchor>1.0)
        anchor-=1.0;
    //qDebug() << anchor;


    //qDebug() << color.red();

    QList<Device> ret;
    foreach(PositionedDevice posDev, positionedDevices){
        float individualAnchor = anchor+(posDev.angle/360);
        while(individualAnchor > 1.0)
            individualAnchor -= 1.0;
        Device dev = posDev.d;
        QColor color = normalizeColor(QColor::fromHsvF(individualAnchor,1,1));
        int lowestChannel = dev.getFirstChannel();

        dev.setChannel(lowestChannel + 0,color.redF());
        dev.setChannel(lowestChannel + 1,color.greenF());
        dev.setChannel(lowestChannel + 2,color.blueF());
        if(dev.getType() == Device::RGBW)
            dev.setChannel(lowestChannel + 3,0.0);

        ret.append(dev);
    }

    stopwatch.restart();

    return ret;
}

QList<Device> ColorWheelScene::getUsedLights()
{
    return usedDevices;
}

void ColorWheelScene::clientRegistered(QJsonObject msg, int id)
{

}

void ColorWheelScene::clientUnregistered(QJsonObject msg, int id)
{

}

void ColorWheelScene::clientMessage(QJsonObject msg, int id)
{

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
    return serializeScene(QJsonObject());
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

/**
 * @brief ColorWheelScene::getSpeed
 * @param time in seconds
 * @return
 */
double ColorWheelScene::getSpeed(double time)
{
    double m = (maxSpeed - basicSpeed) / duration;
    return std::max(maxSpeed - m*time,double(basicSpeed));
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

