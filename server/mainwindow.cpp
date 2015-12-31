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

#include <QJsonArray>
#include <QCoreApplication>

#define SETTING_KEY_MAINSCENE_JSON "mainscenejson"
#define SETTING_KEY_JACKP "jackp"

/*
 *Hier werden alle Szenen sowie die Klasse JackProcessor zur Kommunikation über jack instanziiert.
*/


MainWindow::MainWindow() :
    settings(), usedLamps(),status(),offsetRequested(true),
    availableDevices(), wss(this),jackProcessor(&wss,this),outDevices(),
    timer(this),getChangesRunning(false), sceneBuilder(&wss,&availableDevices,&jackProcessor),
    myBeamerDeviceProvider(&wss,&availableDevices),
    beamerShutterSceneManager(&myBeamerDeviceProvider,&wss,&jackProcessor),  olaDeviceProvider(),
    remoteBeat(&wss,&jackProcessor),mainScene(),filieIoProv(&wss,&mainScene,this)
{
    //availableDevices = Device::loadDevicesFromXml("~/devices.xml");
    //needed for settings
    QCoreApplication::setOrganizationName("fdg");
    QCoreApplication::setOrganizationDomain("fdg-ab.de");
    QCoreApplication::setApplicationName("light master");

    availableDevices.append(Device(0,4,"rgbw1",Device::RGBW,QVector3D(-2,-3,0)));
    availableDevices.append(Device(4,4,"rgbw2",Device::RGBW,QVector3D(-1,1,0)));
    availableDevices.append(Device(8,4,"rgbw3",Device::RGBW,QVector3D(2,-2,0)));
    availableDevices.append(Device(0,6,"beamer1",Device::Beamer,QVector3D(3,0,0)));
    /*
     * Still the devices are memorized hard-coded in the programm.
     * If you change the number of devices at this point, every scene-qml-file has to be edited,
     * because there is a minColorNum for color changing...
     * ...so it might be very nice to introduce a user-friendly gui to configure the devices,
     * saved in a xml-file and parsed at the beginning!
     * This was an explanation in best Jonas' English...
     */


    /*
     * So here is a first step:
     * You can now serialize and deserialize devices to JSON so it is easy to export them into a config file
     * This was an comment in Jakob's English...
     */
    QJsonDocument d;
    d.setArray(Device::serializeDevices(availableDevices));
    qDebug() << d.toJson();

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
    QList<Device> newState;
    getChangesRunning = true;
    QList<Device> changes;

    newState = mainScene.data()->getLights();

    QListIterator<Device> lampIter(availableDevices);
    while(lampIter.hasNext()){
        Device lamp = lampIter.next();
        if(!newState.contains(lamp))//setzte alle unbenutzen Lampen auf 0
            newState.append(lamp);
    }


    //test for changes
    QListIterator<Device> mapIter(newState);
    while(mapIter.hasNext()){
        Device lamp = mapIter.next();
        Device old = lamp.findEqualDevice(status);
        if(!old.valuesEqual(lamp) || offsetRequested){//value changed
            changes.append(lamp);
            status.removeAll(old);
            status.append(lamp);//set status to new value
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
    mainScene = QSharedPointer<DiaScene>(new DiaScene(availableDevices,&wss,&jackProcessor,&sceneBuilder, "main"));
    mainScene.data()->loadSerialized(data);
    mainScene.data()->start();
    usedLamps = mainScene.data()->getUsedLights();

    QListIterator<Device> lampIter(usedLamps);
    while(lampIter.hasNext()){
        Device lamp = lampIter.next();
        status.append(lamp);
    }
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
