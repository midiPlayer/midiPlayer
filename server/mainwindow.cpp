#include "mainwindow.h"
#include "keyframescene.h"
#include <QDebug>
#include <QSetIterator>
#include <QListIterator>
#include "time.h"
#include "math.h"
#include "beatscene1.h"
#include "fusionscene.h"
#include "colorscene.h"
#include "flashscene.h"
#include "whitedevice.h"

#include <QJsonArray>
#include <QCoreApplication>

#include "channeldevicestate.h"

#define SETTING_KEY_MAINSCENE_JSON "mainscenejson"
#define SETTING_KEY_JACKP "jackp"

/*
 *Hier werden alle Szenen sowie die Klasse JackProcessor zur Kommunikation über jack instanziiert.
*/


MainWindow::MainWindow() :
    virtualDevManager(), settings(), status(),offsetRequested(true),
    wss(this),jackProcessor(&wss,this),outDevices(),
    timer(this),getChangesRunning(false), sceneBuilder(&wss,&virtualDevManager,&jackProcessor),
    myBeamerDeviceProvider(&wss,&virtualDevManager),
    beamerShutterSceneManager(&myBeamerDeviceProvider,&wss,&jackProcessor),  olaDeviceProvider(&virtualDevManager),
    remoteBeat(&wss,&jackProcessor),mainScene(),filieIoProv(&wss,&mainScene,this),
    monitorIO(&wss)
{
    //availableDevices = Device::loadDevicesFromXml("~/devices.xml");
    //needed for settings
    QCoreApplication::setOrganizationName("fdg");
    QCoreApplication::setOrganizationDomain("fdg-ab.de");
    QCoreApplication::setApplicationName("light master");


    outDevices.append(&myBeamerDeviceProvider);
    outDevices.append(&olaDeviceProvider);

    connect(&wss,SIGNAL(clientClosed()),this,SLOT(save()));

    connect(&timer,SIGNAL(timeout()),this,SLOT(trigger()));
    timer.setInterval(10);

    loadScenes(getMainScenSettings());

    jackProcessor.initJack(this);
}


//This method is called by jackprocesssor
//if offset is true all lamps will be transmitet not only th changes
void MainWindow::getChanges()
{
    QMap<QString, QSharedPointer<DeviceState> > newState;
    getChangesRunning = true;
    QMap<QString, QSharedPointer<DeviceState> > changes;

    newState = mainScene.data()->getDeviceState();


    QMap<QString, QSharedPointer<Device> > avDev = virtualDevManager.getDevices();
    foreach (QString deviceId, avDev.keys()) {
        if(!newState.contains(deviceId)){//setzte alle unbenutzen Lampen auf 0
            newState.insert(deviceId,avDev.value(deviceId).data()->createEmptyState());
        }
    }

    //single device saver:
    //replaces 0 by 0.001 to keep the single device always warm
    foreach (QString devId, newState.keys()) {
        QSharedPointer<ChannelDeviceState> state = newState.value(devId).dynamicCast<ChannelDeviceState>();
        if(state.isNull())
            continue;
        if(state.data()->device->getType() == Device::White &&
                state.data()->getChannelValue(state.data()->getFirstChannel()) == 0.0 ){
            state.data()->setChannel(state.data()->getFirstChannel(),0.01);
        }
    }


    //test for changes
    changes.clear();
    foreach (QString devId, newState.keys()) {
        newState.value(devId).data()->publish();
        if(offsetRequested || !status.contains(devId) || !status.value(devId).data()->equal(newState.value(devId).data())){
            if(status.contains(devId))
                status.remove(devId);
            status.insert(devId,newState.value(devId));
            changes.insert(devId,newState.value(devId));
        }
    }

    offsetRequested = false;


    foreach (OutputDevice *out,outDevices) {
        out->publish(newState,changes);
    }

    getChangesRunning = false;

}

void MainWindow::loadScenes(QJsonObject data)
{

    wss.disconnectAllClients();

    timer.stop();
    mainScene = QSharedPointer<DiaScene>(new DiaScene(&wss,&jackProcessor,&sceneBuilder,&monitorIO, "main"));
    mainScene.data()->loadSerialized(data);
    mainScene.data()->start();

    timer.start();

}


void MainWindow::trigger()
{
    if(getChangesRunning){
        qDebug() << "still running!";
        return;
    }
    getChanges();
}


QJsonObject MainWindow::getMainScenSettings()
{
    QString settingStr = settings.value(SETTING_KEY_MAINSCENE_JSON).toString();
    QJsonDocument d = QJsonDocument::fromJson(settingStr.toUtf8());
    return d.object();
}

void MainWindow::save()
{
    QJsonDocument d;
    QJsonObject obj = mainScene.data()->serialize();
    d.setObject(obj);
    qDebug() << d.toJson();
    settings.setValue(SETTING_KEY_MAINSCENE_JSON,d.toJson());

    QJsonDocument jackS;
    jackS.setObject(jackProcessor.serialize());
    settings.setValue(SETTING_KEY_JACKP,jackS.toJson());
}

MainWindow::~MainWindow()
{
    save();
}
