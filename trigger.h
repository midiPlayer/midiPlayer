#ifndef TRIGGER_H
#define TRIGGER_H
#include "websocketserverprovider.h"
#include "jackprocessor.h"
#include <QSet>
#include <QObject>
class Trigger : public QObject, public WebSocketServerProvider
{
Q_OBJECT
public:
    Trigger(WebSocketServer *ws,JackProcessor *jackP);
    enum TriggerType{BEAT,TIMER,ONSET};
    void start();
    void stop();
    void clientRegistered(QJsonObject msg,int clientId);
    void clientUnregistered(QJsonObject msg,int clientId);
    void clientMessage(QJsonObject msg,int clientId);
    QString getRequestType();
    QSet<TriggerType> triggerConfig;
public slots:
    void beat();
    void onset();
signals:
    void trigger();
private:
    JackProcessor *jack;
    QJsonObject getState();
};

#endif // TRIGGER_H
