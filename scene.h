#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include<QMap>
#include <QSet>

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QString nameP, QString descP = 0, QObject *parent = 0);
    virtual QMap<int,float> getLights() = 0;
    virtual QSet<int> getUsedLights() = 0;
    virtual int getFadeOutDuration() = 0;
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
