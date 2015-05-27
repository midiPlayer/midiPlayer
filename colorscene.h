#ifndef COLORSCENE_H
#define COLORSCENE_H
#include "scene.h"
#include <QJsonObject>
#include "websocketserverprovider.h"
#include "colorbutton.h"
class ColorScene : public Scene, public WebSocketServerProvider
{
Q_OBJECT
public:
    ColorScene(QList<Device> avDev, WebSocketServer *ws, QString name, QJsonObject serialized = QJsonObject());
    QList<Device> getLights();
    QList<Device> getUsedLights();
    QJsonObject serialize();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg,int clientIdCounter);
    QString getRequestType();
    ColorButton colorButton;
public slots:
    void reloadColor();
private:
    QList<Device> devices;
};

#endif // COLORSCENE_H
