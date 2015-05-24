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
    explicit Scene(QString nameP, QJsonObject serialized = QJsonObject());
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
    virtual QString getSceneTypeString() = 0;

    void setDesc(const QString &value);
    virtual QJsonObject serialize() = 0;

signals:

public slots:

private:
    long startS, startMs;
    QString name;
    QString desc;
protected:
    QJsonObject serializeScene(QJsonObject inherited = QJsonObject());
};

#endif // SCENE_H
