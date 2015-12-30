#include "fileoutput.h"

#include <QFile>
#include <QTextStream>

FileOutput::FileOutput():path(),content()
{

}

void FileOutput::setPath(QString pathP)
{
    path = pathP;
    run();
}

void FileOutput::setContent(QString contentP)
{
    content = contentP;
    run();
}

QString FileOutput::getPath()
{
    return path;
}

QString FileOutput::getContent()
{
    return content;
}

void FileOutput::run()
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
