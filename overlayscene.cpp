#include "overlayscene.h"
#include "fusionscene.h"

OverlayScene::OverlayScene(QString nameP, Scene *sceneP, bool minP):name(nameP),scene(sceneP),min(minP)
{
    scene->resetTime();
}

QList<Device> OverlayScene::getEffect(QList<Device> other)
{
    QList<Device> overlay = scene->getLights();

    FusionScene fusion("fusion");
    fusion.import(other);
    if(min)
        fusion.fusion(overlay,Device::MIN,1.0f);
    else
        fusion.fusion(overlay,Device::MAX,1.0f);
    return fusion.getLights();
}

void OverlayScene::reset()
{
    scene->resetTime();
    scene->start();
}

void OverlayScene::stop()
{
    scene->stop();
}

QString OverlayScene::getName()
{
    return name;
}

bool OverlayScene::stopRequested()
{
    return scene->exitRequested();
}
