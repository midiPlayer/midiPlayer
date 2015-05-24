#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <QJsonObject>

class Serializable
{
public:
    virtual QJsonObject serialize() = 0;
};

#endif // SERIALIZABLE_H
