#ifndef BEATSCENE1_H
#define BEATSCENE1_H
#include "scene.h"
#include "jackprocessor.h"
#include "device.h"
#include <QList>
#include "device.h"
#include <QColor>
#include "websocketserverprovider.h"
#include <QSet>

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
public slots:
    void beat();
    void onset();
private:
    JackProcessor* jackProcessor;
    QList<Device> availableDevices;
    QColor c;
    QColor highlighted;
    QList<QColor> options;
    QList<Device> usedDevices;
    enum TriggerType{BEAT,TIMER,ONSET};
    QSet<TriggerType> foregroundTrigger;
    QSet<TriggerType> backgroundTrigger;

    void changeForeground();
    void changeBackground();
    void parseTriggerMsg(QJsonObject msg, QSet<TriggerType> *trigger);

    QJsonObject getState(bool foreground, bool background);
};

#endif // BEATSCENE1_H
