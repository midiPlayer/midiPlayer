#ifndef FILEIO_H
#define FILEIO_H
#include <QObject>


class FileIO: public QObject
{
Q_OBJECT

public:
    Q_PROPERTY(QString path READ getPath WRITE setPath)
    Q_PROPERTY(QString content READ getContent WRITE setContent)
    FileIO();
    void setPath(QString pathP);
    void setContent(QString contentP);
    QString getPath();
    QString getContent();

private:
    void run();
    QString path;
    QString content;


};

#endif // FILEIO_H
