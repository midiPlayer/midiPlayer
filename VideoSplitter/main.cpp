#include <QCoreApplication>
#include "launcher.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc != 3){
        qDebug() << "usage: numImages numThreads";
        exit(1);
    }

    new Launcher(QString(argv[1]).toInt(0),QString(argv[2]).toInt(0));

    return a.exec();
}

