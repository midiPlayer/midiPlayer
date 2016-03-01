#include "launcher.h"
#include <QDebug>
#include "math.h"

void Launcher::startProcess(int first,int num)
{
    runningProcs++;
    QSharedPointer<myProcess> t = QSharedPointer<myProcess>(new myProcess(first,num,workCmd,procCount++));
    connect(t.data(),SIGNAL(finished()),this,SLOT(workerFinished()));
    connect(t.data(),SIGNAL(error()),this,SLOT(workerError()));
    t.data()->start();
    process.append(t);
}

Launcher::Launcher(int numImages, int numThreads, QString workerCommand) : runningProcs(1),
    timer(this),workCmd(workerCommand),procCount(0)
{
    if(numThreads > numImages)
        numThreads = numImages;// more threads than images are useless!

    int iPerThread = numImages / numThreads; //we roud down here as we use ints

    for(int i = 0; i < (numThreads - 1); i++){
        startProcess(i*iPerThread, iPerThread);
    }
    startProcess((numThreads -1)*iPerThread, numImages - (numThreads -1)*iPerThread);

    connect(&timer,SIGNAL(timeout()),this,SLOT(printProgress()));
    timer.setInterval(1000);
    timer.start();
}

void Launcher::workerFinished()
{
    runningProcs--;
    if(runningProcs == 0){
        qDebug() << "lastThread Finished!";
        exit(0);
    }
}

void Launcher::printProgress()
{
    float totalProgress = 0;
    foreach (QSharedPointer<myProcess> p, process) {
        totalProgress += p.data()->getProgress();
    }
    totalProgress /= process.size();
    qDebug() << ((int)(totalProgress * 10000))/ 100 << "%";
}

void Launcher::workerError()
{
    qDebug() << "error in worker!";
    exit(1);
}
