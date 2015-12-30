#include "fileio.h"

#include <QFile>
#include <QTextStream>

FileIO::FileIO():path(),content()
{

}

void FileIO::setPath(QString pathP)
{
    path = pathP;
    run();
}

void FileIO::setContent(QString contentP)
{
    content = contentP;
    run();
}

QString FileIO::getPath()
{
    return path;
}

QString FileIO::getContent()
{
    return content;
}

void FileIO::run()
{
    if(path.length() != 0 && content.length() != 0){
        QFile f(path);
        if (!f.open(QIODevice::ReadWrite))
               return;
           QTextStream out(&f);
           out.setCodec("UTF-8");
           out << content;
           out.flush();
           f.flush();
           f.close();
    }

}
