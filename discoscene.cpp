#include "discoscene.h"
#include "fusionscene.h"


DiscoScene::DiscoScene(QString name) : Scene(name),fusion("fusion")
{

}



QList<Device> DiscoScene::getLights()
{
    fusion.reset();
    bool first = false;
    foreach (Scene* effect, effects) {
        if(first){
            fusion.import(effect);
        }
        else
            fusion.fusion(effect,Device::AV,0.5f);
    }
    return fusion.getLights();
}

QList<Device> DiscoScene::getUsedLights()
{
    QList<Device> useedLights;
    foreach (Scene* effect, effects) {
        foreach(Device d,effect->getUsedLights()){
            if(useedLights.contains(d))
                useedLights.append(d);
        }
    }

    return useedLights;
}

int DiscoScene::getFadeOutDuration()
{

}
