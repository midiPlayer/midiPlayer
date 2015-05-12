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
    FlashScene(QString name, WebSocketServer *ws, QList<Device> avDevP, JackProcessor *jackP);
    QList<Device>getLights();
    QList<Device> getUsedLights();
    void start();
    void stop();
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
public slots:
    void triggered();
private:
    Trigger trigger;
    QList<Device> avDev;
    QList<Device>flashState;
    bool flash;
    QTime time;
    int smoothDuration;//in ms
    int beatSpeed;
    float timePer;


};

#endif // FLASHSCENE_H
