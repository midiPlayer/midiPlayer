#include "colorpreview.h"
#include "websocketserver.h"

ColorPreview::ColorPreview(WebSocketServer *ws, VirtualDeviceManager *manager): QObject(),WebSocketServerProvider(ws),
    activeClient(-1),selectDevManager(manager,ws),state()
{
    ws->registerProvider(this);
    connect(&selectDevManager,SIGNAL(virtualDevicesChanged()),this,SLOT(regenerate()));
    regenerate();
}

void ColorPreview::clientRegistered(QJsonObject msg, int id)
{
    activeClient = id;
}

void ColorPreview::clientUnregistered(QJsonObject msg, int clientIdCounter)
{
    if(activeClient == clientIdCounter){
        activeClient = -1;
    }
}

void ColorPreview::clientMessage(QJsonObject msg, int clientIdCounter)
{

}

QString ColorPreview::getRequestType()
{
    return "colorPreview";
}

QMap<QString, QSharedPointer<DeviceState> > ColorPreview::getDeviceState()
{
    return state;
}

bool ColorPreview::isActive()
{
    return activeClient != -1;
}

void ColorPreview::regenerate()
{

}
