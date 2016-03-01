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
    Launcher(int numImages, int numThreads, QString workerCommand,QStringList args);

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
    QStringList workArgs;
    int procCount;
};

#endif // LAUNCHER_H
