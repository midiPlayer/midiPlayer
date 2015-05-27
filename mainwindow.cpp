#include "mainwindow.h"
#include "ui_mainwindow.h"
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

#define SETTING_KEY_DISOCSCENE_JSON "discoscenejson"

/*
 *Hier werden alle Szenen sowie die Klasse JackProcessor zur kommunikation über jack  instanziiert.
*/


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),settings(), jackProcessor(this),usedLamps(),status(),offsetRequested(true),
    availableDevices(), wss(this),outDevices(),
    timer(this),getChangesRunning(false), ui(new Ui::MainWindow),
    sceneBuilder(&wss,&availableDevices,&jackProcessor),
    myBeamerDeviceProvider(&wss,&availableDevices),
    beamerShutterSceneManager(&myBeamerDeviceProvider,&wss,&jackProcessor),  olaDeviceProvider(),
    remoteBeat(&wss,&jackProcessor),mainScene()
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

    mainScene = QSharedPointer<DiaScene>(new DiaScene(availableDevices,&wss,&jackProcessor,"main"));

    discoscene = QSharedPointer<DiscoScene>(new DiscoScene(&wss,&sceneBuilder,"disco",getDiscoScenSettings()));

    outDevices.append(&myBeamerDeviceProvider);
    outDevices.append(&olaDeviceProvider);
    //connect(p,SIGNAL(midiRequest()),this,SLOT(getChanges()));
    ui->setupUi(this);

    mainScene.data()->addScene(discoscene,1);
    mainScene.data()->addScene(QSharedPointer<ColorScene>(new ColorScene(availableDevices,"black")),1);
    mainScene.data()->start();



    usedLamps = mainScene.data()->getUsedLights();

    QListIterator<Device> lampIter(usedLamps);
    while(lampIter.hasNext()){
        Device lamp = lampIter.next();
        status.append(lamp);
    }

    //qDebug() << "" << usedLamps;

    jackProcessor.initJack(this);

    connect(&timer,SIGNAL(timeout()),this,SLOT(trigger()));
    timer.setInterval(10);
    timer.start();

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

  /*  //overlay:
    if(currentOverlay != -1){
        OverlayScene* overlay = overlays.at(currentOverlay);
        if(overlay->stopRequested()){
            stopCurrentOverlay();
        }
        else
            newState = overlay->getEffect(newState);
    }*/

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



void MainWindow::trigger()
{
    if(getChangesRunning){
        qDebug() << "still running!";
        return;
    }
    getChanges();
}


QJsonObject MainWindow::getDiscoScenSettings()
{
    QString settingStr = settings.value(SETTING_KEY_DISOCSCENE_JSON).toString();
    QJsonDocument d = QJsonDocument::fromJson(settingStr.toUtf8());
    return d.object();
}


MainWindow::~MainWindow()
{
    QJsonDocument d;
    QJsonObject obj = discoscene.data()->serialize();
    d.setObject(obj);
    qDebug() << d.toJson();
    settings.setValue(SETTING_KEY_DISOCSCENE_JSON,d.toJson());
    delete ui;
}
