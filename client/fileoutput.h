#ifndef FileOutput_H
#define FileOutput_H
#include <QObject>


class FileOutput: public QObject
{
Q_OBJECT

public:
    Q_PROPERTY(QString path READ getPath WRITE setPath)
    Q_PROPERTY(QString content READ getContent WRITE setContent)
    FileOutput();
    void setPath(QString pathP);
    void setContent(QString contentP);
    QString getPath();
    QString getContent();

private:
    void run();
    QString path;
    QString content;


};

#endif // FileOutput_H
