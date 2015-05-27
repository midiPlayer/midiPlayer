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
#include "fusionscene.h"
#include "colorbutton.h"

class BeatScene1 : public Scene, public WebSocketServerProvider
{
    Q_OBJECT
public:
    BeatScene1(QList<Device>avDev, JackProcessor* p, WebSocketServer* ws,QString name,QJsonObject serialized = QJsonObject());
    QList<Device>getLights();
    QList<Device> getUsedLights();
    void stop();
    void start();
    void clientRegistered(QJsonObject msg,int clientIdCounter);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    virtual QJsonObject serialize();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
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
    int smoothDuration;
    QTime smoothTimer;
    FusionScene prev;
    FusionScene next;
    void generateNextScene();
    ColorButton colorButton;

};

#endif // BEATSCENE1_H
