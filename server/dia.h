#ifndef DIA_H
#define DIA_H

#include <QString>
#include <QSharedPointer>
#include "scene.h"
#include "scenebuilder.h"
#include "websocketserverprovider.h"
#include "diascenemonitorcontrol.h"

class Dia : public WebSocketServerProvider
{
public:
    Dia(QJsonObject serialized, SceneBuilder *builder, WebSocketServer *wss, JackProcessor *jackP,MonitorIO *monitorP);
    Dia(QSharedPointer<Scene> sceneP, QString nameP, QString descP, float fadeInDurationP, WebSocketServer *wss, JackProcessor *jackP, MonitorIO *monitorP);
    QSharedPointer<Scene> scene;
    QString name;
    QString desc;
    float fadeInDuration;
    QJsonObject serialize(SceneBuilder *builder);

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();

    void start();
    void stop();

    DiaSceneMonitorControl monitorControl;
};

#endif // DIA_H
