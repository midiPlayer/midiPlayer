#include "musicdevice.h"
#include <QDirIterator>
#include <QDebug>
#include <QStringList>
#include "websocketserver.h"
#include <QJsonArray>

#define MUSIC_DIR "/home/jakob/Musik/MP3/einzelneLieder/"

#define KEY_FILE "musicplayer_file"

MusicDevice::MusicDevice(WebSocketServer *ws, QJsonObject serialized) :
    Device("musicDevice",Device::MusicPlayer,QVector3D(0,0,0)),
    selectedFile(""), WebSocketServerProvider(ws), qtPlayer(), volume(0.0)
{
    ws->registerProvider(this);
    if(serialized.length() != 0){
        selectedFile = serialized.value(KEY_FILE).toString();
    }

    qtPlayer.setVolume(volume * 100);
    if(selectedFile != "")
        qtPlayer.setMedia(QUrl::fromLocalFile(selectedFile));
}

QJsonObject MusicDevice::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_FILE,selectedFile);
    return ret;
}

void MusicDevice::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject ret;
    QJsonArray songs;
    QDirIterator d(MUSIC_DIR,QDirIterator::Subdirectories);
    while (d.hasNext()) {
        QString path = d.next();
        if(!d.fileInfo().isDir()){
            QJsonObject song;
            song.insert("path",path);
            song.insert("name",path.replace(MUSIC_DIR,""));
            songs.append(song);
        }
    }
    ret.insert("songs",songs);
    ret.insert("setPath",selectedFile);
    sendMsg(ret,id,true);
}

void MusicDevice::clientUnregistered(QJsonObject msg, int id)
{
}

void MusicDevice::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("setPath")){
        selectedFile = msg.value("setPath").toString("");
        if(selectedFile != "")
            qtPlayer.setMedia(QUrl::fromLocalFile(selectedFile));
        stop();
        sendMsgButNotTo(msg,id,true);
    }
}

QString MusicDevice::getRequestType()
{
    return "musicPlayer";
}

QSharedPointer<DeviceState> MusicDevice::createEmptyState()
{
    return createEmptyMusicState();
}

QSharedPointer<MusicDeviceState> MusicDevice::createEmptyMusicState()
{
    return QSharedPointer<MusicDeviceState>(new MusicDeviceState(this));
}

void MusicDevice::setVolume(float vol)
{
    volume = vol;
    qtPlayer.setVolume(volume * 100);
}

void MusicDevice::play(quint64 msec)
{
    if(selectedFile == "")//we can't play empty files
        return;

    setPosition(msec);
    qtPlayer.play();
}

void MusicDevice::setPosition(qint64 pos)
{
    qtPlayer.setPosition(pos);
}

void MusicDevice::stop()
{
    qtPlayer.stop();
}
