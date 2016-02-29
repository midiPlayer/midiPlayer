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
    virtual QMap<QString,QSharedPointer<DeviceState> > getDeviceState() = 0;
    QString getName();
    virtual void stop() {};
    virtual void start() {};
    virtual bool exitRequested() { return false;};
    virtual QString getSceneTypeString() = 0;

    virtual QJsonObject serialize() = 0;

signals:

public slots:

private:
    QString name;
protected:
    QJsonObject serializeScene(QJsonObject inherited = QJsonObject());
};

#endif // SCENE_H
