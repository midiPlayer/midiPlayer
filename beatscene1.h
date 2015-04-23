#ifndef BEATSCENE1_H
#define BEATSCENE1_H
#include "scene.h"
#include "jackprocessor.h"
class BeatScene1 : public Scene
{
    Q_OBJECT
public:
    BeatScene1(QString name,JackProcessor* p);
    QMap<int,float>getLights();
    QSet<int> getUsedLights();
    int getFadeOutDuration();
    void stop();
    void start();
public slots:
    void beat();
private:
    JackProcessor* jackProcessor;
    int c;
};

#endif // BEATSCENE1_H
