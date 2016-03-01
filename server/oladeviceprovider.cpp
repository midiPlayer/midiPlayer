#include "oladeviceprovider.h"
#include <QDebug>
#include "channeldevice.h"
#include <QSharedPointer>
OlaDeviceProvider::OlaDeviceProvider(VirtualDeviceManager *manager): filterDevManager(manager), active(false), wrapper()
{
    filterDevManager.addAcceptedType(Device::RGB);
    filterDevManager.addAcceptedType(Device::RGBW);
    filterDevManager.addAcceptedType(Device::White);


    ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);
    if (wrapper.Setup())
        active = true;
    else{
        qDebug() << "Setup failed";
        return;
    }
}

OlaDeviceProvider::~OlaDeviceProvider()
{
    wrapper.Cleanup();
}

void OlaDeviceProvider::publish(QMap<QString, QSharedPointer<DeviceState> > outDevices, QMap<QString, QSharedPointer<DeviceState> > changes)
{
    if(changes.count() == 0)
        return;
    static unsigned int universe = 0;
    ola::DmxBuffer buffer;
    buffer.Blackout();
    foreach (QString devId, outDevices.keys()) {
        if(!filterDevManager.getDevices().contains(devId))
            continue;

        QSharedPointer<ChannelDeviceState> d = outDevices.value(devId).dynamicCast<ChannelDeviceState>();
        if(d.isNull())
           continue;

          foreach(int c,d.data()->getChannels()){
              float cValue = d.data()->getChannelValue(c);
            buffer.SetChannel(c,cValue*255);
        }
    }
    wrapper.GetClient()->SendDMX(universe, buffer, ola::client::SendDMXArgs());
}

bool OlaDeviceProvider::SendData()
{
    static unsigned int universe = 0;
    static unsigned int i = 0;
    ola::DmxBuffer buffer;
    buffer.Blackout();
    buffer.SetChannel(0, i);
    wrapper.GetClient()->SendDMX(universe, buffer, ola::client::SendDMXArgs());
    if (++i == 100) {
    //wrapper->GetSelectServer()->Terminate();
        i=0;
    }
    return true;
}

