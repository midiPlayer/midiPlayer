#ifndef WHITEDEVICE_H
#define WHITEDEVICE_H
#include "channeldevice.h"
#include <QColor>
class WhiteDevice : public ChannelDevice
{
public:
    WhiteDevice(int firstChannel, int numChannels, QString devIdP, QColor colorOfDeviceP = QColor(255,255,255),  QVector3D pos = QVector3D(0,0,0));
    QJsonObject serialize();
    QColor getColorOfDevice();
private:
    QColor colorOfDevice;
};

#endif // WHITEDEVICE_H
