#ifndef BEATSCENE2_H
#define BEATSCENE2_H
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

//Beatscene2: bei jedem Beat wird bei einem Scheinwerfer die Farbe geändert (auf eine ausgewählte, aber von
//anderen Scheinwerfern noch nicht ausgestrahlte Farbe)

class BeatScene2 : public Scene, public WebSocketServerProvider
{
public:
    Q_OBJECT
public:
    BeatScene2(QList<Device>avDev, JackProcessor* p, WebSocketServer* ws,QString name,QJsonObject serialized = QJsonObject());
    QList<Device>getLights();
    QList<Device> getUsedLights();
    void stop();
    void start();
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    virtual QJsonObject serialize();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
public slots:
    void changeColor();
private:
    struct BeatSceneDev{
        Device device;
        QColor color;
    };
    QList<BeatSceneDev> availableDevices;
    QColor c;
    QList<QColor> options;
    Trigger trigger;
    int smoothDuration;
    QTime smoothTimer;
    FusionScene prev;
    FusionScene next;
    void generateNextScene();
    ColorButton colorButton;
    int nextLamp;

};

#endif // BEATSCENE2_H
