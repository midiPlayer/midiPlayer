#include "oladeviceprovider.h"
#include <QDebug>
OlaDeviceProvider::OlaDeviceProvider(): active(false), wrapper()
{
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

void OlaDeviceProvider::publish(QList<Device> outDevices, QList<Device> changes)
{
    if(changes.length() == 0)
        return;
    static unsigned int universe = 0;
    ola::DmxBuffer buffer;
    buffer.Blackout();
    foreach (Device d,outDevices) {
        if(!(d.getType() == Device::RGB || d.getType() == Device::RGBW))
            continue;
          foreach(int c,d.getChannels()){
            buffer.SetChannel(c,d.getChannelValue(c)*255);
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

