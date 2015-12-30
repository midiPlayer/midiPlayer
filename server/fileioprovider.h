#ifndef FILEIOPROVIDER_H
#define FILEIOPROVIDER_H
#include "websocketserverprovider.h"
#include "diascene.h"
#include <QSharedPointer>
class FileIOProvider : public WebSocketServerProvider
{
public:
    FileIOProvider(WebSocketServer *ws,QSharedPointer<DiaScene> *mainSceneP);
    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
private:
    QSharedPointer<DiaScene> *mainScene;
};

#endif // FILEIOPROVIDER_H
