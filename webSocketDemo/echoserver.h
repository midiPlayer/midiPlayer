#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class EchoServer : public QObject
{
    Q_OBJECT
public:
    explicit EchoServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
    ~EchoServer();
signals:
    void numClientsChanged(int num);
    void trigger();
    void valueChanged(int);
public slots:
    void sendValue(int value);
Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void clientsChanged();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;

};

#endif //ECHOSERVER_H
