#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QSharedPointer>
#include "myprocess.h"
#include <QTimer>

class Launcher : public QObject
{
Q_OBJECT

public:
    Launcher(int numImages, int numThreads, QString workerCommand);

    QList<QSharedPointer<myProcess> > process;
    void startProcess(int i, int iPerThread);
public slots:
    void workerFinished();
    void printProgress();
    void workerError();
private:
    int runningProcs;
    QTimer timer;
    QString workCmd;
};

#endif // LAUNCHER_H
