#ifndef BEAMERSHUTTERSCENE_H
#define BEAMERSHUTTERSCENE_H
#include "scene.h"
class BeamerShutterScene
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif // BEAMERSHUTTERSCENE_H
