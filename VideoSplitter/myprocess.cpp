#include "myprocess.h"
#include <QDebug>

myProcess::myProcess(int firstImageP, int numImagesP, QString processCmd, QStringList args, int procIdP) : QObject(),firstImage(firstImageP),
    numImages(numImagesP),worker(this),progress(0.0), procCmd(processCmd), procId(procIdP),procArgs(args)
{
    connect(&worker,SIGNAL(finished(int)),this,SLOT(workerFinished(int)));
    connect(&worker,SIGNAL(readyRead()),this,SLOT(readWorker()));
    connect(&worker,SIGNAL(error(QProcess::ProcessError)),this,SLOT(workerError()));
}

void myProcess::start()
{
//    qDebug() << "process started: first Image:"<<firstImage;
    procArgs << QString::number(firstImage) << QString::number(procId) << QString::number(numImages);
    worker.start(procCmd, procArgs);
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
    QTextStream myteststream(&read);
    int imageNr = 0;
    myteststream >> imageNr;
    progress = (float)(imageNr - firstImage) / (float)numImages;
    emit progressChanged(progress);
}

void myProcess::workerError()
{
    qDebug() << "error!" << worker.errorString();
    emit error();
}



