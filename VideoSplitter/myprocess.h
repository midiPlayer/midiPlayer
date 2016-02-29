#ifndef MYPROCESS_H
#define MYPROCESS_H
#include <QProcess>

class myProcess : public QObject
{    
Q_OBJECT

public:
    myProcess(int firstImageP, int numImagesP, QString processCmd);
    void start();
    float getProgress();

signals:
    void progressChanged(float progress);
    void finished();
    void error();
public slots:
    void workerFinished(int code);
    void readWorker();
    void workerError();
private:
    int firstImage;
    int numImages;
    QProcess worker;
    float progress;
    QString procCmd;

};

#endif // MYPROCESS_H
