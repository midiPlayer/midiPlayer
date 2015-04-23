#include "scene.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <QDebug>

Scene::Scene(QString nameP, QString descP, QObject *parent) : name(nameP),desc(descP),
    QObject(parent)
{

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

