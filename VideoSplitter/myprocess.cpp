#include "myprocess.h"
#include <QDebug>

myProcess::myProcess(int firstImageP, int numImagesP, QString processCmd) : QObject(),firstImage(firstImageP),
    numImages(numImagesP),worker(this),progress(0.0), procCmd(processCmd)
{
    connect(&worker,SIGNAL(finished(int)),this,SLOT(workerFinished(int)));
    connect(&worker,SIGNAL(readyRead()),this,SLOT(readWorker()));
    connect(&worker,SIGNAL(error(QProcess::ProcessError)),this,SLOT(workerError()));
}

void myProcess::start()
{
    qDebug() << "process started: first Image:"<<firstImage;
    QStringList arguments;
    arguments << "" + firstImage << "" + numImages;
    worker.start(procCmd, arguments);
}

float myProcess::getProgress()
{
    return progress;
}

void myProcess::workerFinished(int code)
{
    if(code == 0)
        emit finished();
    else
        emit error();
}

void myProcess::readWorker()
{
    QString read = QString(worker.readAll());
    qDebug() << read;
    progress = (read.toInt() - firstImage) / numImages;
    emit progressChanged(progress);
}

void myProcess::workerError()
{
    qDebug() << "error!" << worker.errorString();
    emit error();
}


