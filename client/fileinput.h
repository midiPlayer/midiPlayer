#ifndef FileInput_H
#define FileInput_H
#include <QObject>


class FileInput: public QObject
{
Q_OBJECT

public:
    Q_PROPERTY(QString path READ getPath WRITE setPath)
    FileInput();
    void setPath(QString pathP);
    QString getPath();

signals:
    void contentRead(QString content);

private:
    QString path;


};

#endif // FileInput_H
