#include "beatscene1.h"
#include <QDebug>
#include "websocketserver.h"

#define KEY_SMOOTHNESS "smoothness"
#define KEY_BACKGROUNDTRIGGER "backgroundTrigger"
#define KEY_COLOR "color"
#define KEY_SELECT_DEV_MANAGER "selectDevManager"

#define l1 12
#define l2 13
#define l3 14

#define MAX_SMOOTHNESS_DUR 2000


BeatScene1::BeatScene1(VirtualDeviceManager *manager, JackProcessor *p, WebSocketServer *ws, QString name, QJsonObject serialized) :
    Scene(name,serialized),WebSocketServerProvider(ws),
    filterDeviceManager(manager),selectDevManager(&filterDeviceManager,ws), c(0,0,0),highlighted(0,0,0),
    backgroundTrigger(ws,p),smoothDuration(200),smoothTimer(),prev("prev"),next("next"), colorButton(ws)
{
    if(serialized.length() != 0){
        if(serialized.contains(KEY_SMOOTHNESS)){
            smoothDuration = serialized.value(KEY_SMOOTHNESS).toInt();
        }
        if(serialized.contains(KEY_COLOR)){
            colorButton.loadSerialized(serialized.value(KEY_COLOR).toObject());
        }
    }

    backgroundTrigger.triggerConfig.insert(Trigger::BEAT );

    connect(&selectDevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(generateNextScene()));

    filterDeviceManager.addAcceptedType(Device::RGB);
    filterDeviceManager.addAcceptedType(Device::RGBW);
    filterDeviceManager.addAcceptedType(Device::Beamer);

    selectDevManager.deserialize(serialized.value(KEY_SELECT_DEV_MANAGER).toObject());

//    next.import();
//    prev.import();

    connect(&backgroundTrigger,SIGNAL(trigger()),this,SLOT(changeBackground()));
    ws->registerProvider(this);
    smoothTimer.start();
}


QMap<QString, QSharedPointer<DeviceState> > BeatScene1::getDeviceState()
{
    if(smoothDuration == 0 || smoothTimer.elapsed() > smoothDuration)
        return next.getDeviceState();
    else{
        float per = float(smoothTimer.elapsed()) / float(smoothDuration);
        if(per>1.0)
            per = 1;
        return prev.passiveFusion(next.getDeviceState(),ChannelDeviceState::AV,per);
    }
}


void BeatScene1::stop()
{
    qDebug() << "stoped";
    backgroundTrigger.stop();
}

void BeatScene1::start()
{
    qDebug() << "start";
    backgroundTrigger.start();
}

void BeatScene1::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject replay;
    QJsonObject config;
    config.insert("backgroundTrigger",backgroundTrigger.providerId);
    config.insert("smoothnessChanged",double(smoothDuration)/double(MAX_SMOOTHNESS_DUR));
    config.insert("colorButton", colorButton.providerId);
    config.insert("selectDevManager", selectDevManager.providerId);
    replay.insert("config",config);
    sendMsg(replay,id,true);
}

void BeatScene1::clientUnregistered(QJsonObject msg, int id)
{

}

void BeatScene1::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("smoothnessChanged")){
        smoothDuration = msg.value("smoothnessChanged").toDouble(0)*MAX_SMOOTHNESS_DUR;
    }
    sendMsgButNotTo(msg,id,true);
}

QString BeatScene1::getRequestType()
{
    return "beatScene1";
}

QJsonObject BeatScene1::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_BACKGROUNDTRIGGER,backgroundTrigger.serialize());
    ret.insert(KEY_COLOR,colorButton.serialize());
    ret.insert(KEY_SMOOTHNESS,smoothDuration);
    ret.insert(KEY_SELECT_DEV_MANAGER,selectDevManager.serialize());
    return serializeScene(ret);
}

QString BeatScene1::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString BeatScene1::getSceneTypeStringStaticaly()
{
    return "beatScene1";
}


void BeatScene1::changeBackground()
{
    QList <QColor> options=colorButton.getColors();
    QColor last = c;
    while((last == c || c == highlighted) && options.length() > 2)
    {
        int i = rand() % options.length();
        c = options.at(i);
    }
    generateNextScene();
}

void BeatScene1::generateNextScene()
{
    //prev.import(getLights());
    QMap<QString, QSharedPointer<DeviceState> > ret;
    QMap<QString, QSharedPointer<Device> > avDevs = selectDevManager.getDevices();
    foreach (QString devId, avDevs.keys()) {
        QSharedPointer<Device> dev = avDevs.value(devId);
        QSharedPointer<DeviceState> state = dev.data()->createEmptyState();
        QSharedPointer<ChannelDeviceState> d = state.dynamicCast<ChannelDeviceState>();
        int firstChannel = d.data()->getFirstChannel();
        d.data()->setChannel(firstChannel + 0,c.red()/255.0f);
        d.data()->setChannel(firstChannel + 1,c.green()/255.0f);
        d.data()->setChannel(firstChannel + 2,c.blue()/255.0f);

        ret.insert(devId,d);
    }
    next.reset();
    next.import(ret);

    smoothTimer.restart();
}



