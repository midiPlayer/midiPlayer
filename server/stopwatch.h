#ifndef STOPWATCH_H
#define STOPWATCH_H
#include <QTimer>
#include "websocketserverprovider.h"
#include <QObject>

class Stopwatch : public QObject, public WebSocketServerProvider
{
Q_OBJECT

public:
    Stopwatch(WebSocketServer *ws);
    void start(bool notify = true);
    void stop(bool notify = true);
    void resume(bool notify = true);
    int getMSecs();
    void setTo(int ms,bool notify = true);

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
private:
    int snaped;
    bool running;
    long time;
    QTimer timer;
    long getTimestamp();
public slots:
    void sendTimeSync();
};

#endif // STOPWATCH_H
