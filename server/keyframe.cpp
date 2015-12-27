#include "keyframe.h"
/**
 * @brief Keyframe::Keyframe
 * A keyframe ist a point that stores the state of a lamp at a time
 */

#define KEY_KEYFRAME_TIME "keyframe_time"
#define KEY_KEYFRAME_STATE "keyframe_state"

Keyframe::Keyframe(double timeP, DeviceState stateP) : state(stateP), time(timeP)
{
}

Keyframe::Keyframe(QJsonObject serialized) : state(serialized.value(KEY_KEYFRAME_STATE).toObject()),
    time(serialized.value(KEY_KEYFRAME_TIME).toInt(0))
{ }

int Keyframe::Compare(const Keyframe &other)
{
    if(time < other.time) return -1;
    if(time > other.time) return 1;
    return 0;
}

DeviceState Keyframe::fusionWith(Keyframe later)
{
    if(state.deviceId != later.state.deviceId){
        throw("cannot fusion!");
    }

    return state.fusionWith(later.state,DeviceState::AV,later.time - time);
}

QJsonObject Keyframe::serialize()
{
    QJsonObject ret;
    ret.insert(KEY_KEYFRAME_TIME,time);
    ret.insert(KEY_KEYFRAME_STATE,state.serialize());
    return ret;
}
