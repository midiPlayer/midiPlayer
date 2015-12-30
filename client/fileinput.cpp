#include "fileinput.h"

#include <QFile>

FileInput::FileInput() : path()
{

}

void FileInput::setPath(QString pathP)
{
    QString content;
    path = pathP;
    if(path.length() != 0){
        QFile f(path);
           if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
               return;

           while (!f.atEnd()) {
               QByteArray line = f.readLine();
               content.append(line);
           }
           emit contentRead(content);
    }
}

QString FileInput::getPath()
{
    return path;
}

