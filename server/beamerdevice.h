#ifndef BEAMERDEVICE_H
#define BEAMERDEVICE_H
#include "channeldevice.h"
#include <QJsonObject>
class BeamerDevice : public ChannelDevice
{
public:
    enum ShutterType{ CIRCULAR , NONE};
    BeamerDevice(ShutterType shutterTypeP = NONE, QJsonObject shutterParamsP = QJsonObject());
    BeamerDevice(int firstChannelP, int numChannelsP, QString devIdP, ShutterType shutterTypeP = NONE, QJsonObject shutterParamsP = QJsonObject());
    ShutterType getShutterType() const;
    QJsonObject shutterParams;
private:
    ShutterType shutterType;

};

#endif // BEAMERDEVICE_H
