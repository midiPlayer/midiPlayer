#ifndef MUSICDEVICE_H
#define MUSICDEVICE_H

#include "serializable.h"
#include <QString>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include "websocketserverprovider.h"
#include <QMediaPlayer>
#include <devicestate.h>
#include <device.h>
#include "musicdevicestate.h"

class MusicDevice : public QObject, public Device, public WebSocketServerProvider
{
Q_OBJECT

public:
    MusicDevice(WebSocketServer *ws,QJsonObject serialized = QJsonObject());
    QJsonObject serialize();

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg, int id);
    void clientMessage(QJsonObject msg, int id);
    QString getRequestType();
    QSharedPointer<DeviceState> createEmptyState();
    QSharedPointer<MusicDeviceState> createEmptyMusicState();
    void setVolume(float vol);

public slots:
    void play(quint64 msec = 0);
    void setPosition(qint64 pos);
    void stop();
private:
    QString selectedFile;
    QMediaPlayer qtPlayer;
    float volume;

};

#endif // MUSICDEVICE_H
