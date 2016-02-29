#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "channeldevicestate.h"
#include "serializable.h"
#include "websocketserverprovider.h"
#include <QObject>
#include <QSharedPointer>

class Keyframe  : public QObject, public Serializable, public WebSocketServerProvider
{
Q_OBJECT

public:
    Keyframe(double timeP, ChannelDeviceState stateP, WebSocketServer *ws);
    Keyframe(QJsonObject serialized, WebSocketServer *ws, VirtualDeviceManager *manager);
    ChannelDeviceState state;
    double time;
    ChannelDeviceState fusionWith(QSharedPointer <Keyframe> later, double now);
    QJsonObject serialize();

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    bool isLiveEditing();
    bool liveEditing;
    void beforeDelete();

signals:
    void deleteRequested(Keyframe *self);
private:
    WebSocketServer *wss;
    int liveEditor;
};

#endif // KEYFRAME_H
