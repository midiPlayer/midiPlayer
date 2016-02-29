#ifndef OUTPUTDEVICE_H
#define OUTPUTDEVICE_H

#include <QMap>
#include "device.h"
class OutputDevice
{
public:
    OutputDevice();
    virtual void publish(QMap<QString, QSharedPointer<DeviceState> > allStates,QMap<QString, QSharedPointer<DeviceState> > changes) = 0;
};

#endif // OUTPUTDEVICE_H
