#ifndef TRIGGER_H
#define TRIGGER_H
#include "websocketserverprovider.h"
#include "jackprocessor.h"
#include <QSet>
#include <QObject>
#include "serializable.h"
#include <QTimer>

class Trigger : public QObject, public WebSocketServerProvider, public Serializable
{
Q_OBJECT

public:
    Trigger(WebSocketServer *ws, JackProcessor *jackP, QJsonObject serialized = QJsonObject());
    enum TriggerType{BEAT,TIMER,ONSET};
    void start();
    void stop();
    void clientRegistered(QJsonObject msg,int clientId);
    void clientUnregistered(QJsonObject msg,int clientId);
    void clientMessage(QJsonObject msg,int clientId);
    QString getRequestType();
    QSet<TriggerType> triggerConfig;
    QJsonObject serialize();
    QJsonObject getTriggerSourceJson();
    void loadSerialized(QJsonObject serialized);
public slots:
    void beat();
    void onset();
    void onTimer();
signals:
    void trigger();
private:
    JackProcessor* jack;
    QJsonObject getState();
    void setState(QJsonObject fgt);
    int numBeats;
    int beatCount;
    void triggerInt();

    int interval;
    float randomness;

    QTimer timer;
    void startTimer();
    void stopTimer();
    bool running;
};

#endif // TRIGGER_H
