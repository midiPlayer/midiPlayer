#include "scene.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <QDebug>
#include <typeinfo>
#define KEY_NAME "sceneName"
#define KEY_DESC "sceneDesc"
#define KEY_SCENE_TYPE "sceneType"

Scene::Scene(QString nameP, QJsonObject serialized) : name(nameP),desc(""),
    QObject(0)
{
    if(serialized.length() != 0){
        name = serialized.value(KEY_NAME).toString();
        desc = serialized.value(KEY_DESC).toString();
    }
}

int Scene::getFadeOutDuration()
{
 return 0;//default
}

void Scene::resetTime()
{
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    startS = s;
    startMs = ms;
}

QString Scene::getName()
{
    return name;
}

QString Scene::getDesc()
{
    return desc;
}

int Scene::getDeltaTime()
{
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    long deltaS = s- startS;
    long deltaMs = ms-startMs;
    deltaMs = 1000 + deltaMs;
    return deltaMs + deltaS * 1000;;
}

QJsonObject Scene::serialize(QJsonObject inherited)
{
    inherited.insert(KEY_NAME,name);
    inherited.insert(KEY_DESC,desc);
    return  Serializable::serialize(inherited);
}
void Scene::setDesc(const QString &value)
{
    desc = value;
}


/*
 * Problem: I have no Idea how to find an elegant way to dynamicly generate the scenes by a name string as
 * I can't find any elegant way to provide an name string staticly
 * */


