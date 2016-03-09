#include "beatscene1.h"
#include <QDebug>
#include "websocketserver.h"
#include "math.h"

#define KEY_SMOOTHNESS "smoothness"
#define KEY_BACKGROUNDTRIGGER "backgroundTrigger"
#define KEY_COLOR "color"
#define KEY_SELECT_DEV_MANAGER "selectDevManager"
#define KEY_NUM_CHANGING_DEVS "changingDevices"
#define KEY_SAME_COLOR "sameColor"

#define l1 12
#define l2 13
#define l3 14

#define MAX_SMOOTHNESS_DUR 2000


BeatScene1::BeatScene1(VirtualDeviceManager *manager, JackProcessor *p, WebSocketServer *ws, QString name, QJsonObject serialized) :
    Scene(name,serialized),WebSocketServerProvider(ws),
    filterDeviceManager(manager),selectDevManager(&filterDeviceManager,ws),
    backgroundTrigger(ws,p),smoothDuration(200),smoothTimer(),prev("prev"),next("next"), colorButton(ws),
    numChangingDevices(0), sameColor(true)
{
    if(serialized.length() != 0){
        if(serialized.contains(KEY_SMOOTHNESS)){
            smoothDuration = serialized.value(KEY_SMOOTHNESS).toInt();
        }
        if(serialized.contains(KEY_COLOR)){
            colorButton.loadSerialized(serialized.value(KEY_COLOR).toObject());
        }
    }

    numChangingDevices = serialized.value(KEY_NUM_CHANGING_DEVS).toInt(numChangingDevices);
    sameColor = serialized.value(KEY_SAME_COLOR).toBool(sameColor);

    backgroundTrigger.triggerConfig.insert(Trigger::BEAT );

    connect(&selectDevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(devicesChanged()));

    filterDeviceManager.addAcceptedType(Device::RGB);
    filterDeviceManager.addAcceptedType(Device::RGBW);
    filterDeviceManager.addAcceptedType(Device::Beamer);

    selectDevManager.deserialize(serialized.value(KEY_SELECT_DEV_MANAGER).toObject());

//    next.import();
//    prev.import();

    connect(&backgroundTrigger,SIGNAL(trigger()),this,SLOT(generateNextScene()));
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
    config.insert("numDevs",selectDevManager.getDevices().count());
    config.insert(KEY_NUM_CHANGING_DEVS,numChangingDevices);
    config.insert(KEY_SAME_COLOR,sameColor);
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
    if(msg.contains(KEY_NUM_CHANGING_DEVS)){
        numChangingDevices =  round(msg.value(KEY_NUM_CHANGING_DEVS).toDouble(numChangingDevices));
    }
    if(msg.contains(KEY_SAME_COLOR)){
        sameColor = msg.value(KEY_SAME_COLOR).toBool(sameColor);
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
    ret.insert(KEY_NUM_CHANGING_DEVS,numChangingDevices);
    ret.insert(KEY_SAME_COLOR,sameColor);
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

void BeatScene1::devicesChanged()
{
    QJsonObject config;
    config.insert("numDevs",selectDevManager.getDevices().count());
    QJsonObject msg;
    msg.insert("config",config);
    sendMsg(msg,true);
    generateNextScene();
}

void BeatScene1::generateNextScene()
{
    QMap<QString, QSharedPointer<DeviceState> > ret;
    QMap<QString, QSharedPointer<Device> > avDevs = selectDevManager.getDevices();

    //choose devices
    QList<QString> freeDevs = selectDevManager.getDevices().keys();
    QList<QString> chosenDevs;
    if(numChangingDevices == 0)
        chosenDevs = freeDevs;
    else{
        for (int i = 0; i < numChangingDevices && freeDevs.count() > 0 ; i++) {
            int index = rand() % freeDevs.count();
            chosenDevs.append(freeDevs.at(index));
            freeDevs.removeAt(index);
        }
    }

    QColor commonColor = getNextColor();

    foreach (QString devId, avDevs.keys()) {

        //not chosen devices ceep their old state
        if(!chosenDevs.contains(devId) && next.getDeviceState().contains(devId)){
            ret.insert(devId,next.getDeviceState().value(devId));
            continue;
        }

        QSharedPointer<Device> dev = avDevs.value(devId);
        QSharedPointer<DeviceState> state = dev.data()->createEmptyState();
        QSharedPointer<ChannelDeviceState> d = state.dynamicCast<ChannelDeviceState>();
        int firstChannel = d.data()->getFirstChannel();
        if(sameColor){
            d.data()->setChannel(firstChannel + 0,commonColor.red()/255.0f);
            d.data()->setChannel(firstChannel + 1,commonColor.green()/255.0f);
            d.data()->setChannel(firstChannel + 2,commonColor.blue()/255.0f);
        }
        else{
            do{
                QColor c = getNextColor();
                d.data()->setChannel(firstChannel + 0,c.red()/255.0f);
                d.data()->setChannel(firstChannel + 1,c.green()/255.0f);
                d.data()->setChannel(firstChannel + 2,c.blue()/255.0f);
            }while(colorButton.getColors().length() > 2 &&
                   next.getDeviceState().contains(devId) &&
                   next.getDeviceState().value(devId).data()->equal(d.data()));
        }

        ret.insert(devId,d);
    }
    prev.import(next.getDeviceState());
    next.reset();
    next.import(ret);

    smoothTimer.restart();
}

QColor BeatScene1::getNextColor()
{
    QList <QColor> options=colorButton.getColors();
    int i = rand() % options.length();
    return options.at(i);
}



