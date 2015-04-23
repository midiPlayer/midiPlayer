#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene1.h"
#include "scene2.h"
#include "scene4.h"
#include "keyframescene.h"
#include <QDebug>
#include <QSetIterator>
#include <QListIterator>
#include "time.h"
#include "math.h"
#include "beatscene1.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),scenes(),overlays(),usedLamps(),status(),currentScene(0),currentOverlay(-1),offsetRequested(true),fading(0),nextOnMusic(false),overlayOnMusic(false),
    ui(new Ui::MainWindow)
{
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

    scenes.append(new Scene4("black"));
    scenes.append(new BeatScene1("beat",p));
    scenes.append(new KeyFrameScene("/media/daten/Jakob/Schule/FDG/12/theater/scene1.playable","overtuere",true,"Das ist die Beschreibung zur Overtuere un es ist schoen, dass du dir so viel zeit nimmst das zu Lesen!"));
    scenes.append(new Scene1("asdf"));
    scenes.append(new Scene2("soso"));
    scenes.at(currentScene)->resetTime();
    scenes.at(currentScene)->start();


    overlays.append(new OverlayScene("magic1",new KeyFrameScene("/media/daten/Jakob/Schule/FDG/12/theater/scene1.playable","overtuere",true,"Das ist die Beschreibung zur Overtuere un es ist schoen, dass du dir so viel zeit nimmst das zu Lesen!"),false));
    foreach (OverlayScene* overlay, overlays) {
        ui->selectOverlay->addItem(overlay->getName(),overlays.indexOf(overlay));
    }

    QListIterator<Scene*> scenesIter(scenes);
    while(scenesIter.hasNext()){
        Scene* scene = scenesIter.next();
        usedLamps += scene->getUsedLights();
    }
    QSetIterator<int> lampIter(usedLamps);
    while(lampIter.hasNext()){
        int lamp = lampIter.next();
        status.insert(lamp,0.0f);
    }

    updateSceneLables();

    qDebug() << "" << usedLamps;

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

QMap<int, float> MainWindow::getChanges()//if offset is true all lamps will be transmitet not only th changes
{
    QMap<int,float> changes;
    Scene* scene = scenes.at(currentScene);
    QMap<int,float> newState;

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
            Scene* nextScene = scenes.at(currentScene+fading);
            newState = sceneFusion(scene->getLights(),nextScene->getLights(),per,usedLamps);
        }

    }



    if(fading ==0){
         newState = scene->getLights();
         if(scene->exitRequested()){
             nextScene();
         }
    }


    QSetIterator<int> lampIter(usedLamps);
    while(lampIter.hasNext()){
        int lamp = lampIter.next();
        if(!newState.contains(lamp))//setzte alle unbenutzen Lampen auf 0
            newState.insert(lamp,0.0f);
    }

    //overlay:
    if(currentOverlay != -1){
        OverlayScene* overlay = overlays.at(currentOverlay);
        if(overlay->stopRequested()){
            stopCurrentOverlay();
        }
        else
            newState = overlay->getEffect(newState);
    }

    //test for changes
    QMapIterator<int,float> mapIter(newState);
    while(mapIter.hasNext()){
        mapIter.next();
        int lamp = mapIter.key();
        float old = status.value(lamp);
        if(mapIter.value() != old || offsetRequested){//value changed

            changes.insert(lamp,mapIter.value());
            status.remove(lamp);
            status.insert(lamp,mapIter.value());//set status to new value
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




QMap<int, float> MainWindow::sceneFusion(QMap<int, float> sceneA, QMap<int, float> sceneB, float ballance,QSet<int> usedLamps)
{
    QMap<int, float> res;
    QSetIterator<int> it(usedLamps);
    while (it.hasNext()) {
        int lamp = it.next();
        float lampA = 0;
        float lampB = 0;
        if(sceneA.contains(lamp))
            lampA = sceneA.value(lamp);
        if(sceneB.contains(lamp))
            lampB = sceneB.value(lamp);
        float lampRes = (1.0-ballance)*lampA + ballance * lampB;
        res.insert(lamp,lampRes);
    }
    return res;
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
