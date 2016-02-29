#include "scene.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <QDebug>
#include <typeinfo>
#define KEY_NAME "sceneName"
#define KEY_DESC "sceneDesc"
#define KEY_SCENE_TYPE "sceneType"

Scene::Scene(QString nameP, QJsonObject serialized) : name(nameP), QObject(0)
{
    if(serialized.length() != 0){
        name = serialized.value(KEY_NAME).toString();
    }
}


QString Scene::getName()
{
    return name;
}


QJsonObject Scene::serializeScene(QJsonObject inherited)
{
    inherited.insert(KEY_NAME,name);
    return  inherited;
}
