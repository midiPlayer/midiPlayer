#include "musicplayer.h"
#include <QDirIterator>
#include <QDebug>
#include <QStringList>
#include "websocketserver.h"
#include <QJsonArray>

#define MUSIC_DIR "/home/jakob/Musik/MP3/einzelneLieder/"

#define KEY_FILE "musicplayer_file"


MusicPlayer::MusicPlayer(WebSocketServer *ws, QJsonObject serialized) : selectedFile(""), playing(false),
    player(this), WebSocketServerProvider(ws)
{
    ws->registerProvider(this);
    if(serialized.length() != 0){
        selectedFile = serialized.value(KEY_FILE).toString();
    }
}

QJsonObject MusicPlayer::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_FILE,selectedFile);
    return ret;
}

void MusicPlayer::play(int secs)
{
    if(selectedFile == "")//we can't play empty files
        return;

    if(playing)
        stop();
    QStringList args;
    args << selectedFile;

    player.start("/usr/bin/vlc",args);
    playing = true;
}

void MusicPlayer::stop()
{
    playing = false;
    if(player.state() != QProcess::NotRunning){
        player.terminate();
    }
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
        stop();
        sendMsgButNotTo(msg,id,true);
    }
}

QString MusicPlayer::getRequestType()
{
    return "musicPlayer";
}
