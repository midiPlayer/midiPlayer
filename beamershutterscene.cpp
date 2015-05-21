#include "beamershutterscene.h"
#include "websocketserver.h"





BeamerShutterScene::BeamerShutterScene(QString nameP, WebSocketServer *ws):WebSocketServerProvider(ws),name(nameP)
{

}

QString BeamerShutterScene::getName()
{
 return name;
}
