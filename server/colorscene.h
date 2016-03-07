#ifndef COLORSCENE_H
#define COLORSCENE_H
#include "scene.h"
#include <QJsonObject>
#include "websocketserverprovider.h"
#include "colorbutton.h"
#include "virtualdevicemanager.h"
#include "selectvirtualdevicemanager.h"
#include "filtervirtualdevicemanager.h"

class ColorScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT

public:
    ColorScene(VirtualDeviceManager *vDevManager, WebSocketServer *ws, QString name, QJsonObject serialized = QJsonObject());
    QJsonObject serialize();
    QMap<QString,QSharedPointer<DeviceState> > getDeviceState();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    ColorButton colorButton;
public slots:
    void reloadDevices();
    void reloadColor();
private:
    QMap<QString,QSharedPointer<DeviceState> > deviceStates;
    FilterVirtualDeviceManager filterVdevManager;
    SelectVirtualDeviceManager selectDevManager;
};

#endif // COLORSCENE_H
