#include "beatscene1.h"
#include <QDebug>

#define l1 12
#define l2 13
#define l3 14


BeatScene1::BeatScene1(QString name, QList<Device> avDev, JackProcessor* p) : Scene(name), c(0,0,0),highlighted(0,0,0) , availableDevices(avDev),options(),usedDevices()
{
    jackProcessor = p;
    options.append(QColor(255,0,0));
    options.append(QColor(0,255,0));
    options.append(QColor(0,0,255));
    options.append(QColor(255,255,0));
    options.append(QColor(255,0,255));
    options.append(QColor(0,255,255));


    usedDevices.clear();
    foreach (Device d, availableDevices) {
        if(d.getType() == Device::Beamer){
            usedDevices.append(d);
        }
    }
}

QList<Device> BeatScene1::getLights()
{
    QList<Device> ret;
    foreach (Device d, usedDevices) {
        d.setChannel(0,c.red());
        d.setChannel(1,c.green());
        d.setChannel(2,c.blue());

        d.setChannel(3,highlighted.red());
        d.setChannel(4,highlighted.green());
        d.setChannel(5,highlighted.blue());
        ret.append(d);
    }
    return ret;
}

QList<Device> BeatScene1::getUsedLights()
{
    return usedDevices;
}


void BeatScene1::stop()
{
    qDebug() << "stoped";
    disconnect(jackProcessor,SIGNAL(beatNotification()),this,SLOT(beat()));
    disconnect(jackProcessor,SIGNAL(onsetNotification()),this,SLOT(onset()));
}

void BeatScene1::start()
{
    qDebug() << "start";
    connect(jackProcessor,SIGNAL(beatNotification()),this,SLOT(beat()));
    connect(jackProcessor,SIGNAL(onsetNotification()),this,SLOT(onset()));
}

void BeatScene1::beat()
{
    QColor last = c;
    while(last == c || c == highlighted)
    {
        int i = double(options.length())*double(rand())/RAND_MAX - 0.0001;
        c = options.at(i);
    }
    qDebug() << c;
}

void BeatScene1::onset()
{
    QColor last = highlighted;
    while(last == highlighted|| c == highlighted)
    {
        int i = double(options.length())*double(rand())/RAND_MAX - 0.0001;
        highlighted = options.at(i);
    }
    qDebug() << c;
}



