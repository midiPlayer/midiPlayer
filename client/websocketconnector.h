#ifndef WEBSOCKETCONNECTOR_H
#define WEBSOCKETCONNECTOR_H

#include <QObject>
class WebSocketConnector : public QObject
{
Q_OBJECT
    Q_PROPERTY(QString url READ url  WRITE startConnection)
public:
    WebSocketConnector(QObject* parent = 0);
    void startConnection(const QString &);
    QString url() const;
};

#endif // WEBSOCKETCONNECTOR_H
