#if 0
#include "keyframescene.h"
#include <QDebug>
#include <QFile>
KeyFrameScene::KeyFrameScene(QMap<int, QMap<int, float> > frames, QString name, bool autoExitP, QString desc) : keyframes(frames),usedLamps(),Scene(name,desc),exitRequestedV(false),autoExit(autoExitP)
{


    QMapIterator<int,QMap<int,float> > kIt(keyframes);
    while(kIt.hasNext()){
        kIt.next();
        usedLamps += kIt.value().keys();
    }
}

KeyFrameScene::KeyFrameScene(QString path, QString name, bool autoExitP, QString desc): Scene(name,desc),autoExit(autoExitP)
{
    if(path == "")
        return;
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QDataStream *in = new QDataStream(&file);
    QMap<int, QMap<int,float> > playable;
    *(in)  >> playable;
    qDebug() << playable;

    keyframes = playable;

    QMapIterator<int,QMap<int,float> > kIt(keyframes);
    while(kIt.hasNext()){
        kIt.next();
        usedLamps += kIt.value().keys();
    }

}

QList<Device> KeyFrameScene::getLights()
{
    int dTime = getDeltaTime();
    QMap<int,float> prev;
    int prevTime = -1;
    int nextTime = -1;
    QMap<int,float> next;

    QMapIterator<int, QMap<int,float> > iter(keyframes);
    int bestL(-1);
    int bestH(INT_MAX);
    while(iter.hasNext()){
        iter.next();
        int time = iter.key();
        if(time < dTime && time > bestL){
            bestL = time;
            prevTime = time;
            prev = iter.value();
        }
        else if(time > dTime && time < bestH){
            bestH = time;
            nextTime = time;
            next = iter.value();
        }
    }

    if(prevTime == -1 && nextTime == -1)//no kexframes exists
        return QMap<int,float>();
    else if(nextTime == -1){//at the end
        if(autoExit)
            exitRequestedV = true;
        return prev;
    }
    else if(prevTime == -1)//at the start
        return next;
    else{//bentween two keyframes
        float per = ((double)(dTime-prevTime) / (double)(nextTime - prevTime));
        return keyFrameFusion(prev,next,per,usedLamps);
    }
}

QList<Device> KeyFrameScene::getUsedLights()
{
    return usedLamps;
}

int KeyFrameScene::getFadeOutDuration()
{
    return 1000;
}

bool KeyFrameScene::exitRequested()
{
    return exitRequestedV;
}

void KeyFrameScene::start()
{
    exitRequestedV = false;
}

QMap<int, float> KeyFrameScene::keyFrameFusion(QMap<int, float> sceneA, QMap<int, float> sceneB, float ballance,QList<int> usedLamps)
{
    QMap<int, float> res;
    QListIterator<int> it(usedLamps);
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
#endif
