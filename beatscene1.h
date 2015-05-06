#ifndef BEATSCENE1_H && 0
#define BEATSCENE1_H
#include "scene.h"
#include "jackprocessor.h"
#include "device.h"
#include <QList>
#include "device.h"
class BeatScene1 : public Scene
{
    Q_OBJECT
public:
    BeatScene1(QString name,QList<Device>avDev,JackProcessor* p);
    QList<Device>getLights();
    QList<Device> getUsedLights();
    int getFadeOutDuration();
    void stop();
    void start();
public slots:
    void beat();
private:
    JackProcessor* jackProcessor;
    QList<Device> availableDevices;
    int c;
};

#endif // BEATSCENE1_H
