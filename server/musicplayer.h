#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include "serializable.h"
#include <QString>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include "websocketserverprovider.h"
class MusicPlayer : public QObject, public Serializable, public WebSocketServerProvider
{
Q_OBJECT

public:
    MusicPlayer(WebSocketServer *ws,QJsonObject serialized = QJsonObject());
    QJsonObject serialize();

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
public slots:
    void play(int secs);
    void stop();
private:
    QString selectedFile;
    bool playing;
    QProcess player;

};

#endif // MUSICPLAYER_H
