#ifndef COLORSCENE_H
#define COLORSCENE_H
#include "scene.h"
#include <QJsonObject>

class ColorScene : public Scene
{
public:
    ColorScene(QList<Device> avDev,QString name,QJsonObject serialized = QJsonObject());
    QList<Device> getLights();
    QList<Device> getUsedLights();
    QJsonObject serialize();
    QString getSceneTypeString();
    static QString getSceneTypeStringStaticaly();
private:
    QList<Device> devices;
};

#endif // COLORSCENE_H
