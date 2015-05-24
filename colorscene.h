#ifndef COLORSCENE_H
#define COLORSCENE_H
#include "scene.h"
#include <QJsonObject>

class ColorScene : public Scene
{
public:
    ColorScene(QString name, QList<Device> avDev);
    ColorScene(QJsonObject serialized, QList<Device> avDev);
    QList<Device> getLights();
    QList<Device> getUsedLights();
    QJsonObject serialize(QJsonObject inherited = QJsonObject());
    QString getSceneTypeString();
    void init();
private:
    QList<Device> devices;
};

#endif // COLORSCENE_H
