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

#define SETTING_KEY_MAINSCENE_JSON "mainscenejson"
#define SETTING_KEY_JACKP "jackp"

/*
 *Hier werden alle Szenen sowie die Klasse JackProcessor zur kommunikation über jack  instanziiert.
*/


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),settings(), usedLamps(),status(),offsetRequested(true),
    availableDevices(), wss(this),jackProcessor(&wss,this),outDevices(),
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

    mainScene = QSharedPointer<DiaScene>(new DiaScene(availableDevices,&wss,&jackProcessor,&sceneBuilder, "main"));
    mainScene.data()->loadSerialized(getMainScenSettings());
    //discoscene = QSharedPointer<DiscoScene>(new DiscoScene(&wss,&sceneBuilder,"disco",getDiscoScenSettings()));

    outDevices.append(&myBeamerDeviceProvider);
    outDevices.append(&olaDeviceProvider);
    //connect(p,SIGNAL(midiRequest()),this,SLOT(getChanges()));
    ui->setupUi(this);

    connect(ui->saveBtn,SIGNAL(clicked(bool)),this,SLOT(save()));

    //mainScene.data()->addScene(discoscene,"disco1","Das ist die erste Discoscene!",1);
    //mainScene.data()->addScene(QSharedPointer<ColorScene>(new ColorScene(availableDevices,&wss,"black")),"black","eben einfach schwarz - schlicht und doch aufdringlich",1);
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
    delete ui;
}
