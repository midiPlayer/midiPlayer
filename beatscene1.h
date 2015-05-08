#ifndef BEATSCENE1_H
#define BEATSCENE1_H
#include "scene.h"
#include "jackprocessor.h"
#include "device.h"
#include <QList>
#include "device.h"
#include <QColor>
class BeatScene1 : public Scene
{
    Q_OBJECT
public:
    BeatScene1(QString name,QList<Device>avDev,JackProcessor* p);
    QList<Device>getLights();
    QList<Device> getUsedLights();
    void stop();
    void start();
public slots:
    void beat();
    void onset();
private:
    JackProcessor* jackProcessor;
    QList<Device> availableDevices;
    QColor c;
    QColor highlighted;
    QList<QColor> options;
    QList<Device> usedDevices;
};

#endif // BEATSCENE1_H
