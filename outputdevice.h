#ifndef OUTPUTDEVICE_H
#define OUTPUTDEVICE_H

#include <QList>
#include "device.h"
class OutputDevice
{
public:
    OutputDevice();
    virtual void publish(QList<Device> devices) = 0;
};

#endif // OUTPUTDEVICE_H
