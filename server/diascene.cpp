#include "diascene.h"
#include "websocketserver.h"
#include <QJsonArray>
#define KEY_DIAS "dias"
DiaScene::DiaScene(WebSocketServer *ws,
                   JackProcessor *jackP, SceneBuilder *builderP, MonitorIO* monitorIoP, QString name, QJsonObject serialized):
                Scene(name,serialized),WebSocketServerProvider(ws),
                current(-1),fadingTo(-1),fadeTimer(),fusion("fusion"),wss(ws),builder(builderP),
                jack(jackP), monitorIo(monitorIoP), sceneIdCountr(0)
{
    ws->registerProvider(this);
    connect(jack,SIGNAL(musicNotification()),this,SLOT(music()));
}


QMap<QString, QSharedPointer<DeviceState> > DiaScene::getDeviceState()
{
    fusion.reset();

    if(current != -1)
        fusion.import(scenes.value(current).data()->scene.data());
    if(fadingTo != -1){
        QSharedPointer<Dia> fadingToDia  =scenes.value(fadingTo);
        float percentage = fadeTimer.elapsed()/(fadingToDia.data()->fadeInDuration*1000);
        if(percentage> 1){
            percentage = 1;
            if(current != -1)
                scenes.value(current).data()->stop();
            current = fadingTo;
            fadingTo = -1;
        }
        fusion.fusion(fadingToDia.data()->scene,ChannelDeviceState::AV,percentage);
    }
    return fusion.getDeviceState();
}

void DiaScene::clientRegistered(QJsonObject msg, int id)
{
    sendMsg(getState(true),id,false);
}

void DiaScene::clientUnregistered(QJsonObject msg, int id)
{

}

void DiaScene::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("resetScene")){
        if(current != -1){
            scenes.value(order.at(current)).data()->stop();
            scenes.value(order.at(current)).data()->start();
        }
        if(fadingTo != -1){
            scenes.value(order.at(fadingTo)).data()->stop();
            scenes.value(order.at(fadingTo)).data()->start();
        }
        sendMsgButNotTo(msg,id,false);
    }
    if(msg.contains("nextScene")){
        next();
        sendMsg(getState(false),false);
    }
    if(msg.contains("currentScene")){
        int newId = msg.value("currentScene").toInt(-1);
        if(newId != -1 && newId < scenes.count() && newId != current){
            newId = order.indexOf(newId);
            fadeTimer.restart();
            if(fadingTo != -1)
                scenes.value(order.at(fadingTo)).data()->stop();
            scenes.value(order.at(newId)).data()->start();
            fadingTo = newId;
        }
        sendMsgButNotTo(getState(false),id,false);
    }
    if(msg.contains("prevScene")){
        if(scenes.count() != 0 && current > 0){
            fadeTimer.restart();
            if(fadingTo != -1)
                scenes.value(order.at(fadingTo)).data()->stop();
            scenes.value(order.at(current-1)).data()->start();
            fadingTo = current - 1;
        }
        sendMsg(getState(false),false);
    }

    if(msg.contains("addScene")){
        QJsonObject addCmd = msg.value("addScene").toObject();
        QSharedPointer<Scene> newScene = builder->build(addCmd.value("type").toString(),addCmd.value("name").toString());
        if(newScene.isNull()){
           qDebug("duscoScene: ERROR: unknown scene type");
           return;
        }
        addScene(newScene,addCmd.value("name").toString(),"",1.0);
    }
    if(msg.contains("deleteScene")){
        int id = msg.value("deleteScene").toInt(-1);
        if(current != -1 && order.at(current) == id){
            scenes.value(id).data()->stop();
            current = fadingTo == -1 ? -1 : fadingTo;
        }
        if(fadingTo != -1 && order.at(fadingTo) == id){
            scenes.value(id).data()->stop();
            fadingTo = -1;
        }
        if(scenes.contains(id)){
            scenes.remove(id);
            order.removeAll(id);
        }
        sendMsg(getState(true),false);
    }
    if(msg.contains("musicNotification")){
        setNextOnMusic(msg.value("musicNotification").toBool(false));
        sendMsgButNotTo(msg,id,false);
    }

    if(msg.contains("orderChanged")){
        QJsonArray readOrder = msg.value("orderChanged").toArray();
        if(readOrder.size() != order.size()){
            qDebug() << "ERROR: incomplete order!";
            return;
        }
        QList<int> newOrder;
        int newCurrent = current;
        int newFadingTo = fadingTo;
        int ctr = 0;
        foreach (QJsonValue val, readOrder) {
            int i = val.toInt(-1);
            qDebug() << "old current:" << current << "  --> index:" << order.value(current);
            if(i == -1 || i >= order.size()){
                qDebug() << "ERROR: invalid item!";
                return;
            }
            if(current != -1 && order.value(current) == i)
                newCurrent = ctr;
            if(fadingTo != -1 && order.value(fadingTo) == i)
                newFadingTo = ctr;
            newOrder.append(i);
            ctr++;
        }

        order = newOrder;
        current = newCurrent;
        fadingTo = newFadingTo;
        sendMsgButNotTo(msg,id,false);

        qDebug() << "new Current:" << current;
        qDebug() << "current id:" << order.value(current);
    }
}

QString DiaScene::getRequestType()
{

    return "diaScene";
}

void DiaScene::stop()
{
    if(current != -1)
        scenes.value(order.at(current)).data()->stop();
    if(fadingTo != -1)
        scenes.value(order.at(fadingTo)).data()->stop();
}

void DiaScene::start()
{
    if(current != -1)
        scenes.value(order.at(current)).data()->start();
    if(fadingTo != -1)
        scenes.value(order.at(fadingTo)).data()->start();
    if(current == -1 && fadingTo == -1 && scenes.count() > 0){
        current = 0;
        scenes.value(order.at(current)).data()->start();
    }
}

QJsonObject DiaScene::serialize()
{
    QJsonObject serialized;

    QJsonArray dias;
    foreach(int index,order){
        QSharedPointer<Dia> dia = scenes.value(index);
        dias.append(dia.data()->serialize(builder));
    }
    serialized.insert(KEY_DIAS,dias);
    return serializeScene(serialized);
}

QString DiaScene::getSceneTypeString()
{
    return getSceneTypeStringStaticaly();
}

QString DiaScene::getSceneTypeStringStaticaly()
{
    return "diaScene";
}

void DiaScene::addScene(QSharedPointer<Scene> scene,QString name,QString desc,float fadeInDuration)
{
    addScene(QSharedPointer<Dia>(new Dia(scene,name,desc,fadeInDuration,wss,jack,monitorIo)));
}

void DiaScene::addScene(QSharedPointer<Dia> dia)
{

    scenes.insert(sceneIdCountr,dia);
    order.append(sceneIdCountr);
    sceneIdCountr++;
    sendMsg(getState(true),false);
}

void DiaScene::loadSerialized(QJsonObject serialized)
{
    if(serialized.contains(KEY_DIAS))
    foreach (QJsonValue diaVal, serialized.value(KEY_DIAS).toArray()) {
        QJsonObject diaJson = diaVal.toObject();
        QSharedPointer<Dia> scene = QSharedPointer<Dia>(
                    new Dia(diaJson,builder,wss,jack,monitorIo));
        if(!(scene.isNull() || scene.data()->scene.isNull()))
            addScene(scene);
    }
}

void DiaScene::music()
{
    if(nextOnMusic){
        next();
        sendMsg(getState(false),false);
        nextOnMusic = false;
        QJsonObject msg;
        msg.insert("musicNotification",nextOnMusic);
        sendMsg(msg,false);
    }
}

QJsonObject DiaScene::getState(bool addScenes)
{
    QJsonObject msg;
    if(addScenes){
        QJsonArray dias;
        for (int i = 0; i < order.length();i++) {
            QSharedPointer<Dia> dia = scenes.value(order.at(i));
            QJsonObject diaObj;
            diaObj.insert("id",order.at(i));
            diaObj.insert("requestId",dia.data()->providerId);
            diaObj.insert("name",dia.data()->name);
            dias.append(diaObj);
        }
        msg.insert("scenes",dias);
    }
    if(current != -1)
        msg.insert("currentScene",order.at((fadingTo == -1 ? current : fadingTo)));
    return msg;
}

void DiaScene::next()
{
    if(order.length() != 0 && current < order.length() -1){
        fadeTimer.restart();
        if(fadingTo != -1)
            scenes.value(order.at(fadingTo)).data()->stop();
        scenes.value(order.at(current+1)).data()->start();
        fadingTo = current + 1;
    }
}

void DiaScene::setNextOnMusic(bool enable)
{
    qDebug() << "mssic norif " << enable;
    nextOnMusic = enable;
    if(enable)
        jack->requestMusicNotification();
}
