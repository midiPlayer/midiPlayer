#ifndef BEAMERDEVICE_H
#define BEAMERDEVICE_H
#include "device.h"
#include <QJsonObject>
class BeamerDevice : public Device
{
public:
    enum ShutterType{ CIRCULAR , NONE};
    BeamerDevice(ShutterType shutterTypeP = NONE, QJsonObject shutterParamsP = QJsonObject());
    BeamerDevice(QMap<int,float> channelsP, QString devIdP, ShutterType shutterTypeP = NONE, QJsonObject shutterParamsP = QJsonObject());
    BeamerDevice(const BeamerDevice &d);
    BeamerDevice(const BeamerDevice *d);
    ShutterType getShutterType() const;
    QJsonObject shutterParams;
private:
    ShutterType shutterType;
};

#endif // BEAMERDEVICE_H
