#ifndef DIASCENEMONITORCONTROL_H
#define DIASCENEMONITORCONTROL_H
#include "jackprocessor.h"
#include "trigger.h"
#include "websocketserverprovider.h"
#include <QString>
#include "monitorio.h"
#include <QObject>
#include "scene.h"

class DiaSceneMonitorControl: public QObject, public WebSocketServerProvider
{
Q_OBJECT

public:
    DiaSceneMonitorControl(WebSocketServer *ws, JackProcessor *jackP, MonitorIO *monitorP, float* fadeInDurP, QJsonObject serialized = QJsonObject());
    void start();

    void clientRegistered(QJsonObject msg,int clientId);
    void clientUnregistered(QJsonObject msg,int clientId);
    void clientMessage(QJsonObject msg,int clientId);
    QString getRequestType();

    void loadSerialized(QJsonObject serialized);
public slots:
    void triggered();
    QJsonObject serialize(bool includeTrigger = true);
private:
    Trigger trigger;
    QList<QString> resources;

    bool random;

    float* fadeInDur;

    double beatFadeTime;
    MonitorIO *monitor;

    int currentRes;

};

#endif // DIASCENEMONITORCONTROL_H
