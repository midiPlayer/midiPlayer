#ifndef COLORPREVIEW_H
#define COLORPREVIEW_H

#include "websocketserverprovider.h"
#include <QObject>
#include "selectvirtualdevicemanager.h"

class ColorPreview : public QObject, public WebSocketServerProvider
{
Q_OBJECT

public:
    ColorPreview(WebSocketServer *ws,VirtualDeviceManager *manager);
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    QMap<QString,QSharedPointer<DeviceState> > getDeviceState();
    bool isActive();
public slots:
    void regenerate();
private:
    int activeClient;
    SelectVirtualDeviceManager selectDevManager;
    QMap<QString, QSharedPointer<DeviceState> > state;
};

#endif // COLORPREVIEW_H
