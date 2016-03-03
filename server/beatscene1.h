#ifndef BEATSCENE1_H
#define BEATSCENE1_H
#include "scene.h"
#include "jackprocessor.h"
#include "device.h"
#include <QList>
#include "device.h"
#include <QColor>
#include "websocketserverprovider.h"
#include "trigger.h"
#include "fusionscene.h"
#include "colorbutton.h"
#include "filtervirtualdevicemanager.h"
#include "selectvirtualdevicemanager.h"

class BeatScene1 : public Scene, public WebSocketServerProvider
{
    Q_OBJECT

public:
    BeatScene1(VirtualDeviceManager *manager, JackProcessor* p, WebSocketServer* ws,QString name,QJsonObject serialized = QJsonObject());
    QList<Device>getLights();
    QList<Device> getUsedLights();
    QMap<QString,QSharedPointer<DeviceState> > getDeviceState();

    void stop();
    void start();
    void clientRegistered(QJsonObject msg,int clientIdCounter);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    virtual QJsonObject serialize();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
public slots:
    void changeBackground();
private:
    QColor c;
    QColor highlighted;
    Trigger backgroundTrigger;
    int smoothDuration;
    QTime smoothTimer;
    FusionScene prev;
    FusionScene next;
    void generateNextScene();
    ColorButton colorButton;
    FilterVirtualDeviceManager filterDeviceManager;
    SelectVirtualDeviceManager selectDevManager;

};

#endif // BEATSCENE1_H
