#ifndef SCREENCOLORSCENE_H
#define SCREENCOLORSCENE_H

#include "scene.h"
#include "screencolorscenescanner.h"
#include "virtualdevicemanager.h"
#include "filtervirtualdevicemanager.h"
#include "selectvirtualdevicemanager.h"
#include <QMap>

class ScreenColorScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT

public:
    ScreenColorScene(VirtualDeviceManager *manager, WebSocketServer *ws, QString name, QJsonObject serialized);
    QJsonObject serialize();
    QMap<QString,QSharedPointer<DeviceState> > getDeviceState();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();

public slots:
    void setColors(QList<QColor> colors);
    void updateDevices();

private:
    ScreenColorSceneScanner scanner;
    FilterVirtualDeviceManager filterDevManager;
    SelectVirtualDeviceManager selectDevManager;
    QMap<QString, QSharedPointer<DeviceState> > state;
    QList<QColor> colors;
    void updateColor();
};

#endif // SCREENCOLORSCENE_H
