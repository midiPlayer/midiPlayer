#ifndef BEATSCENE1_H
#define BEATSCENE1_H
#include "scene.h"
#include "jackprocessor.h"
#include "device.h"
#include <QList>
#include "device.h"
#include <QColor>
#include "websocketserverprovider.h"
#include "trigger.h"

class BeatScene1 : public Scene, public WebSocketServerProvider
{
    Q_OBJECT
public:
    BeatScene1(QString name, QList<Device>avDev, JackProcessor* p, WebSocketServer *ws);
    QList<Device>getLights();
    QList<Device> getUsedLights();
    void stop();
    void start();
    void clientRegistered(QJsonObject msg,int clientIdCounter);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    QJsonObject serialize();
    QString getSceneTypeString();
public slots:
    void changeForeground();
    void changeBackground();
private:
    QList<Device> availableDevices;
    QColor c;
    QColor highlighted;
    QList<QColor> options;
    QList<Device> usedDevices;
    Trigger foregroundTrigger;
    Trigger backgroundTrigger;
};

#endif // BEATSCENE1_H
