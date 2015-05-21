#ifndef CIRCULARBEAMERSHUTTERSCENE_H
#define CIRCULARBEAMERSHUTTERSCENE_H

#include <QObject>
#include "beamershutterscene.h"
#include "beamerdeviceprovider.h"
#include "trigger.h"
#include "jackprocessor.h"

class CircularBeamerShutterScene : public QObject, public BeamerShutterScene
{
Q_OBJECT
public:
    CircularBeamerShutterScene(QString nameP, beamerDeviceProvider *providerP, WebSocketServer *ws, JackProcessor *p);
    void start();
    void stop();

    void clientRegistered(QJsonObject msg,int clientIdCounter);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg, int clientId);
    QString getRequestType();
    void dataChanged();
public slots:
    void triggered();
private:
    beamerDeviceProvider *provider;
    Trigger trigger;
    bool isRunning;
    float x,y,radius;
    int oldPos;
};

#endif // CIRCULARBEAMERSHUTTERSCENE_H
