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
    Q_PROPERTY(QString requestTypePassive READ getRequestType  WRITE registerForRequestTypePassivly)
    Q_PROPERTY(int requestId READ getReqestId  WRITE setRequestId)
    Q_PROPERTY(QString send READ empty  WRITE sendMessge)
    Q_PROPERTY(bool reopen READ getReopen  WRITE setReopen)
    Q_PROPERTY(QString registrationParams READ getRegistrationParamsAsJson  WRITE setRegistrationParams)
    Q_PROPERTY(bool connected READ isConnected)

public:
    WebSocketConnector(QObject* parent = 0);
    virtual ~WebSocketConnector();
    void startConnection(const QString &url);
    QString url() const;
    QString empty() const;
    QString getRegistrationParamsAsJson();
    QJsonObject getRegistrationParams();
    void setRegistrationParams(QString message);
    WebInterface &webInterface;
    QString getRequestType();
    int getReqestId();
    void setRequestId(int rqID);
    void registerForRequestType(QString type);
    void registerForRequestTypePassivly(QString type);
    void onMessage(QJsonObject msg);
    void sendMessge(QString msg);
    void setReopen(bool reopen);
    bool getReopen();
    bool isPassive();
    bool isConnected();

public slots:
    void connected();
    void disconnected();
signals:
    void message(QJsonObject msg);
    void connectionFailed();
    void connectionSucceded();
private:
    QString urlV;
    QString requestType;
    int requestId;
    QJsonObject requestParams;
    bool passiveConnected;
};

#endif // WEBSOCKETCONNECTOR_H
