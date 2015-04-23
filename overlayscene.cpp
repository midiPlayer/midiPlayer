#include "overlayscene.h"
OverlayScene::OverlayScene(QString nameP, Scene *sceneP, bool minP):name(nameP),scene(sceneP),min(minP)
{
    scene->resetTime();
}

QMap<int, float> OverlayScene::getEffect(QMap<int, float> other)
{
    QMap<int, float> overlay = scene->getLights();
    QMap<int, float> res;
    foreach (int lampI, overlay.keys()) {
        float lampV = overlay.value(lampI);
        float value = lampV;
        if(other.contains(lampI)){
            float otherV = other.value(lampI);
            if((!min && otherV > value) || (min && otherV < value))//max / min
                value = otherV;
        }
        res.insert(lampI,value);
    }
    foreach (int lampI, other.keys()) {
        if(!res.contains(lampI)){
            res.insert(lampI,other.value(lampI));
        }
    }

    return res;
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
