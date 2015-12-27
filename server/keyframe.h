#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "devicestate.h"
#include "serializable.h"
class Keyframe  :Serializable
{
public:
    Keyframe(double timeP, DeviceState stateP);
    Keyframe(QJsonObject serialized);
    int Compare (const Keyframe& other);
    DeviceState state;
    double time;
    DeviceState fusionWith(Keyframe later);
    QJsonObject serialize();
private:
};

#endif // KEYFRAME_H
