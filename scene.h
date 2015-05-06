#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include "device.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QString nameP, QString descP = 0, QObject *parent = 0);
    virtual QList<Device> getLights() = 0;
    virtual QList<Device> getUsedLights() = 0;
    virtual int getFadeOutDuration();
    void resetTime();
    QString getName();
    QString getDesc();
    int getDeltaTime(void);
    virtual void stop() {};
    virtual void start() {};
    virtual bool exitRequested() { return false;};

signals:

public slots:

private:
    long startS, startMs;
    QString name;
    QString desc;
};

#endif // SCENE_H
