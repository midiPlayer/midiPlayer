#include "mediamanager.h"
#include <QDirIterator>
#include <QDebug>
#include <QUrl>

#define RES_DIR "Res/"

MediaManager::MediaManager() : lasResName()
{
}

void MediaManager::searchRes(QString name)
{
    qDebug() << "media search for " << name;
    QDirIterator d(RES_DIR,QDirIterator::Subdirectories);
    QString video;
    QString image;
    while (d.hasNext()) {
        d.next();
        if(!d.fileInfo().isDir()){
            if(d.fileInfo().baseName() != name)
                continue;//no the res we search
            QString suffix = d.fileInfo().suffix().toLower();
            if(suffix == "mp4" || suffix == "avi" || suffix == "webm" || suffix == "mov"){//File is video?; you can add more types here
                video = d.fileInfo().absoluteFilePath();
            }
            else{//file is Imge
                image = d.fileInfo().absoluteFilePath();
            }
        }
    }
    if(video != ""){
        if(image != ""){//error
            emit videoFound(QUrl(video),QUrl(image));
        }
        else{
            emit searchFailed("no Thumbnail found for \"" + name  + "\"");
        }
    }
    else if(image != ""){//image
        emit imageFound(QUrl(image));
    }
    else{
        emit searchFailed("no file found for \"" + name  + "\"");
    }

}

QString MediaManager::getLastResName()
{
    return lasResName;
}
