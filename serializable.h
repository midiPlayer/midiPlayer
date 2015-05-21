#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <QJsonObject>

class Serializable
{
public:
    virtual QJsonObject serialize(QJsonObject inherited = QJsonObject());
};

#endif // SERIALIZABLE_H
