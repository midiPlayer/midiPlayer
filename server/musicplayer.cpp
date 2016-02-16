#include "musicplayer.h"
#include <QDirIterator>
#include <QDebug>
#include <QStringList>
#include "websocketserver.h"
#include <QJsonArray>

#define MUSIC_DIR "/home/jakob/Musik/MP3/einzelneLieder/"

#define KEY_FILE "musicplayer_file"


MusicPlayer::MusicPlayer(WebSocketServer *ws, QJsonObject serialized) : selectedFile(""), playing(false),
    WebSocketServerProvider(ws), qtPlayer()
{
    ws->registerProvider(this);
    if(serialized.length() != 0){
        selectedFile = serialized.value(KEY_FILE).toString();
    }

    qtPlayer.setVolume(50);
    if(selectedFile != "")
        qtPlayer.setMedia(QUrl::fromLocalFile(selectedFile));

}

QJsonObject MusicPlayer::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_FILE,selectedFile);
    return ret;
}

void MusicPlayer::play(quint64 msec)
{
    if(selectedFile == "")//we can't play empty files
        return;

    setPosition(msec);
    qtPlayer.play();
}

void MusicPlayer::setPosition(qint64 pos)
{
    qtPlayer.setPosition(pos);
}

void MusicPlayer::stop()
{
    qtPlayer.stop();
}

void MusicPlayer::clientRegistered(QJsonObject msg, int id)
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

void MusicPlayer::clientUnregistered(QJsonObject msg, int id)
{

}

void MusicPlayer::clientMessage(QJsonObject msg, int id)
{
    if(msg.contains("setPath")){
        selectedFile = msg.value("setPath").toString("");
        if(selectedFile != "")
            qtPlayer.setMedia(QUrl::fromLocalFile(selectedFile));
        stop();
        sendMsgButNotTo(msg,id,true);
    }
}

QString MusicPlayer::getRequestType()
{
    return "musicPlayer";
}

