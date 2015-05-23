#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include "device.h"
#include <QJsonObject>
#include "serializable.h"

class Scene : public QObject, Serializable
{
    Q_OBJECT
public:
    explicit Scene(QString nameP, QString descP = 0, QObject *parent = 0);
    explicit Scene(QJsonObject serialized);
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
    virtual QJsonObject serialize(QJsonObject inherited = QJsonObject());
    virtual QString getSceneTypeString() = 0;

signals:

public slots:

private:
    long startS, startMs;
    QString name;
    QString desc;
};

#endif // SCENE_H
