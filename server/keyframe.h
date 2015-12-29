#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "devicestate.h"
#include "serializable.h"
#include "websocketserverprovider.h"
#include <QObject>
#include <QSharedPointer>

class Keyframe  : public QObject, public Serializable, public WebSocketServerProvider
{
Q_OBJECT

public:
    Keyframe(double timeP, DeviceState stateP, WebSocketServer *ws);
    Keyframe(QJsonObject serialized, WebSocketServer *ws);
    DeviceState state;
    double time;
    DeviceState fusionWith(QSharedPointer <Keyframe> later, double now);
    QJsonObject serialize();

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();

signals:
    void deleteRequested(Keyframe *self);
private:
    WebSocketServer *wss;
};

#endif // KEYFRAME_H
