#ifndef FLASHSCENE_H
#define FLASHSCENE_H
#include "scene.h"
#include "websocketserverprovider.h"
#include "jackprocessor.h"
#include "trigger.h"
#include <QTime>

class FlashScene : public Scene, public WebSocketServerProvider
{
    Q_OBJECT
public:
    FlashScene(WebSocketServer* ws, QList<Device> avDevP, JackProcessor *jackP,QString name,QJsonObject serialized);
    QList<Device>getLights();
    QList<Device> getUsedLights();
    void start();
    void stop();
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    QJsonObject serialize();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
public slots:
    void triggered();
private:
    Trigger trigger;
    QList<Device> availableDevices;
    QList<Device>flashState;
    bool flashEnabled;
    QTime time;
    int smoothDuration;//in ms
    int flashDuration;//in ms
    int beatSpeed;
    float timePer;
    void init(WebSocketServer* ws);
};

#endif // FLASHSCENE_H
