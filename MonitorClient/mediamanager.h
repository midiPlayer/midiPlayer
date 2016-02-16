#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

#include <QString>
#include <QObject>
#include <QUrl>

class MediaManager : public QObject
{
Q_OBJECT

    Q_PROPERTY(QString search READ getLastResName WRITE searchRes)

public:
    MediaManager();
public slots:
    void searchRes(QString name);
    QString getLastResName();
signals:
    void videoFound(QUrl videoUrl, QUrl imageUrl);
    void imageFound(QUrl imageUrl);
    void searchFailed(QString error);
private:
    QString lasResName;
};

#endif // MEDIAMANAGER_H
