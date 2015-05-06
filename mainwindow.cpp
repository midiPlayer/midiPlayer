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
#include "blackscene.h"


/*
 *Hier werden alle Szenen sowie die Klasse JackProcessor zur kommunikation über jack  instanziiert.
*/


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),scenes(),overlays(),usedLamps(),status(),currentScene(0),currentOverlay(-1),offsetRequested(true),fading(0),nextOnMusic(false),overlayOnMusic(false),availableDevices(),
    ui(new Ui::MainWindow)
{
    //availableDevices = Device::loadDevicesFromXml("~/devices.xml");

    QMap<int,float> channels;
    channels.insert(1,0.0f);
    channels.insert(2,0.0f);
    channels.insert(3,0.0f);
    availableDevices.append(Device(channels));

    p = new JackProcessor(this);
    //connect(p,SIGNAL(midiRequest()),this,SLOT(getChanges()));
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(testMidi()));
    connect(ui->nextBtn,SIGNAL(clicked()),this,SLOT(nextScene()));
    connect(ui->prevBtn,SIGNAL(clicked()),this,SLOT(prevScene()));
    connect(p,SIGNAL(musicNotification()),this,SLOT(onMusic()));
    connect(ui->nextWidthNusic,SIGNAL(stateChanged(int)),this,SLOT(requestNextOnMusic(int)));
    connect(ui->playOverlayWidthMusic,SIGNAL(stateChanged(int)),this,SLOT(requestOverlayOnMusic(int)));
    connect(ui->jumpBtn,SIGNAL(clicked()),this,SLOT(jumpClicked()));
    connect(ui->playOverlay,SIGNAL(clicked()),this,SLOT(playOverlayBtn()));

    scenes.append(new BlackScene("black",availableDevices));
    //scenes.append(new BeatScene1("beat",availableDevices,p));
   // scenes.append(new BeatScene1("beat",availableDevices,p));
//    scenes.append(new KeyFrameScene("/media/daten/Jakob/Schule/FDG/12/theater/scene1.playable","overtuere",true,"Das ist die Beschreibung zur Overtuere un es ist schoen, dass du dir so viel zeit nimmst das zu Lesen!"));
    scenes.at(currentScene)->resetTime();
    scenes.at(currentScene)->start();


   // overlays.append(new OverlayScene("magic1",new KeyFrameScene("/media/daten/Jakob/Schule/FDG/12/theater/scene1.playable","overtuere",true,"Das ist die Beschreibung zur Overtuere un es ist schoen, dass du dir so viel zeit nimmst das zu Lesen!"),false));
    foreach (OverlayScene* overlay, overlays) {
        ui->selectOverlay->addItem(overlay->getName(),overlays.indexOf(overlay));
    }

    QListIterator<Scene*> scenesIter(scenes);
    while(scenesIter.hasNext()){
        Scene* scene = scenesIter.next();
        usedLamps += scene->getUsedLights();
    }
    QListIterator<Device> lampIter(usedLamps);
    while(lampIter.hasNext()){
        Device lamp = lampIter.next();
        status.append(lamp);
    }

    updateSceneLables();

    //qDebug() << "" << usedLamps;

    p->initJack(this);

    foreach (Scene* scene, scenes) {
        ui->sceneSelector->addItem(scene->getName(),scenes.indexOf(scene));
    }

}

MainWindow::~MainWindow()
{
    delete p;
    delete ui;
}

void MainWindow::testMidi()
{
    //p->pushCommand(1,0.5);
    offsetRequested = true;
}

//This method is called by jackprocesssor
//if offset is true all lamps will be transmitet not only th changes
QList<Device> MainWindow::getChanges()
{
    QList<Device> changes;
    Scene* scene = scenes.at(currentScene);
    QList<Device> newState;

    if(fading != 0){
        int dur;
        if(fading > 0)
            dur = scene->getFadeOutDuration();
        else
            dur = scenes.at(currentScene-1)->getFadeOutDuration();
        float per = getTimeSinceFadePercentage(dur);
        qDebug() << "percentage: " << per;
        if(per < 0){//error
            resetFadeStart();
            return changes;
        }
        else if(per > 1){//fading done;
            scene->stop();
            currentScene +=fading;
            fading = 0;
            scene = scenes.at(currentScene);
            updateSceneLables();
        }
        else{
            FusionScene fusion("fusion");
            fusion.import(scenes.at(currentScene));
            Scene* nextScene = scenes.at(currentScene+fading);
            fusion.fusion(nextScene,Device::AV,per);
            newState = fusion.getLights();
        }

    }



    if(fading ==0){
         newState = scene->getLights();
         if(scene->exitRequested()){
             nextScene();
         }
    }


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




    return changes;
}

void MainWindow::nextScene()
{
    if(currentScene < scenes.length()-1){
        scenes.at(currentScene+1)->resetTime();
        scenes.at(currentScene+1)->start();
        //currentScene++;
        resetFadeStart();
        fading = 1;//foreward
        updateSceneLables();
    }
    qDebug() << "new Scene: " << currentScene;
}

void MainWindow::prevScene()
{
    if(currentScene > 0){
        scenes.at(currentScene-1)->resetTime();
        scenes.at(currentScene-1)->start();
        //currentScene--;
        resetFadeStart();
        fading = -1;//foreward
        updateSceneLables();
    }
    qDebug() << "new Scene: " << currentScene;
}

void MainWindow::onMusic()
{
    if(nextOnMusic){
        ui->nextWidthNusic->setCheckState(Qt::Unchecked);
        nextScene();
    }
    if(overlayOnMusic){
        ui->playOverlayWidthMusic->setCheckState(Qt::Unchecked);
        if(currentOverlay == -1)
            playOverlay();

    }
}

void MainWindow::jumpClicked()
{
    scenes.at(currentScene)->stop();
    int index = (ui->sceneSelector->itemData(ui->sceneSelector->currentIndex())).toInt();
    Scene* scene = scenes.at(index);
    scene->resetTime();
    scene->start();
    currentScene = index;
    fading = 0;
    updateSceneLables();
}

void MainWindow::playOverlay()
{
    stopCurrentOverlay();
    int index = ui->selectOverlay->itemData(ui->selectOverlay->currentIndex()).toInt();
    OverlayScene* overlay = overlays.at(index);
    overlay->reset();
    currentOverlay = index;
    ui->playOverlay->setText(tr("stop"));
    ui->selectOverlay->setEnabled(false);
    ui->playOverlayWidthMusic->setCheckState(Qt::Unchecked);
    ui->playOverlayWidthMusic->setEnabled(false);
}

void MainWindow::playOverlayBtn()
{
    if(currentOverlay == -1)
        playOverlay();
    else
        stopCurrentOverlay();
}

void MainWindow::requestNextOnMusic(int state)
{
    nextOnMusic = state != 0;
    p->requestMusicNotification();
}

void MainWindow::requestOverlayOnMusic(int state)
{
    overlayOnMusic = state != 0;
    p->requestMusicNotification();
}



void MainWindow::resetFadeStart()
{
    int            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    fadeStartS  = spec.tv_sec;
    fadeStartMs = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
}



float MainWindow::getTimeSinceFadePercentage(int duration)
{
        int            ms; // Milliseconds
        time_t          s;  // Seconds
        struct timespec spec;

        clock_gettime(CLOCK_REALTIME, &spec);

        s  = spec.tv_sec;
        ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
        int deltaS = s- fadeStartS;
        int deltaMs = ms-fadeStartMs;
        int done = deltaMs + deltaS * 1000;
        double percentage = (double)done / (double)duration;
        return (float)percentage;
}

void MainWindow::updateSceneLables()
{
    int n,p;
    if(fading == 0){
        n= currentScene + 1;
        p = currentScene - 1;
    }
    else if(fading > 0){
        n = currentScene +2;
        p = currentScene -1;
    }
    else{
        n = currentScene +1;
        p = currentScene -2;
    }

    if(fading == 0){
        Scene* currentSceneP = scenes.at(currentScene);
        ui->pushButton->setText(currentSceneP->getName());
        ui->descLable->setText(currentSceneP->getDesc());
    }
    else
        ui->pushButton->setText("Fading from:" + scenes.at(currentScene)->getName() + " to: " + scenes.at(currentScene + fading)->getName());
    ui->pushButton->setEnabled(fading == 0);

    if(n < scenes.length()){
        Scene* nextScene = scenes.at(n);
        ui->nextBtn->setText(nextScene->getName());
        ui->nextBtn->setEnabled(fading == 0);
    }
    else{
        ui->nextBtn->setText("-/-");
        ui->nextBtn->setEnabled(false);
    }

    if(p >= 0){
        Scene* prevScene = scenes.at(p);
        ui->prevBtn->setText(prevScene->getName());
        ui->prevBtn->setEnabled(fading == 0);
    }
    else{
        ui->prevBtn->setText("-/-");
        ui->prevBtn->setEnabled(false);
    }

}

void MainWindow::stopCurrentOverlay()
{
    if(currentOverlay != -1){ //cancel current playing
        OverlayScene* overlay = overlays.at(currentOverlay);
        overlay->stop();
        currentOverlay = -1;
    }
    ui->playOverlay->setText(tr("play"));
    ui->selectOverlay->setEnabled(true);
    ui->playOverlayWidthMusic->setEnabled(true);
}
