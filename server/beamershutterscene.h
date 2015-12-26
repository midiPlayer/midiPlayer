#ifndef BEAMERSHUTTERSCENE_H
#define BEAMERSHUTTERSCENE_H
#include "scene.h"
#include "websocketserverprovider.h"
class BeamerShutterScene : public WebSocketServerProvider
{
public:
    BeamerShutterScene(QString nameP,WebSocketServer *ws);
    virtual void start() = 0;
    virtual void stop() = 0;
    QString getName();
private:
    QString name;
};

#endif // BEAMERSHUTTERSCENE_H
