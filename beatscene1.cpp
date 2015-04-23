#include "beatscene1.h"
#include <QDebug>

#define l1 12
#define l2 13
#define l3 14


BeatScene1::BeatScene1(QString name,JackProcessor* p) : Scene(name), c(0)
{
    jackProcessor = p;
}

QMap<int, float> BeatScene1::getLights()
{
    QMap<int, float> ret;
    switch (c) {
    case 0:
        ret.insert(l1,1.0);
        ret.insert(l2,0);
        ret.insert(l3,0);
        break;
    case 1:
        ret.insert(l1,0);
        ret.insert(l2,1.0);
        ret.insert(l3,0);
        break;
    case 2:
        ret.insert(l1,0);
        ret.insert(l2,0);
        ret.insert(l3,1.0);
        break;
    case 3:
        ret.insert(l1,0.5);
        ret.insert(l2,0.5);
        ret.insert(l3,0.5);
        break;
    case 4:
        ret.insert(l1,1.0);
        ret.insert(l2,1.0);
        ret.insert(l3,0);
        break;
    case 5:
        ret.insert(l1,0);
        ret.insert(l2,1.0);
        ret.insert(l3,1.0);
        break;
    case 6:
        ret.insert(l1,1.0);
        ret.insert(l2,1.0);
        ret.insert(l3,1.0);
        break;
    case 7:
        ret.insert(l1,1.0);
        ret.insert(l2,0.5);
        ret.insert(l3,0.5);
        break;
    case 8:
        ret.insert(l1,0.5);
        ret.insert(l2,1.0);
        ret.insert(l3,0.5);
        break;
    case 9:
        ret.insert(l1,0.5);
        ret.insert(l2,0.5);
        ret.insert(l3,1.0);
        break;
    default:
        ret.insert(l1,0);
        ret.insert(l2,0);
        ret.insert(l3,0);
        break;
    }
    return ret;
}

QSet<int> BeatScene1::getUsedLights()
{
    QSet<int> set;
    set << l1;
    set << l2;
    set << l3;
    return set;
}

int BeatScene1::getFadeOutDuration()
{
    return 100;
}

void BeatScene1::stop()
{
    qDebug() << "stoped";
    disconnect(SLOT(beat()));//TODO: fix
}

void BeatScene1::start()
{
    qDebug() << "start";
    connect(jackProcessor,SIGNAL(beatNotification()),this,SLOT(beat()));
}

void BeatScene1::beat()
{
    int last = c;
    while(last == c)
    {
        c = ((float)rand() / (float)RAND_MAX)*10.0f;
        if(c > 9)
           c=9;
    }
    qDebug() << c;
}

