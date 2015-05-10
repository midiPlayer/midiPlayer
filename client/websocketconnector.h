#ifndef WEBSOCKETCONNECTOR_H
#define WEBSOCKETCONNECTOR_H

#include <QObject>
#include "webinterface.h"
#include <QTimer>
#include <QJsonObject>
class WebSocketConnector : public QObject
{
Q_OBJECT
    Q_PROPERTY(QString url READ url  WRITE startConnection)
    Q_PROPERTY(QString requestType READ getRequestType  WRITE registerForRequestType)
    Q_PROPERTY(QString send READ empty  WRITE sendMessge)

public:
    WebSocketConnector(QObject* parent = 0);
    void startConnection(const QString &);
    QString url() const;
    QString empty() const;
    WebInterface &webInterface;
    QString getRequestType();
    void registerForRequestType(QString type);
    void onMessage(QJsonObject msg);
    void sendMessge(QString msg);
public slots:
    void connected();
    void disconnected();
signals:
    void message(QJsonObject msg);
    void connectionFailed();
    void connectionSucceded();
private:
    QString requestType;
};

#endif // WEBSOCKETCONNECTOR_H
