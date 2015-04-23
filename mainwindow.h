#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QSet>
#include "scene.h"
#include "overlayscene.h"
#include "jackprocessor.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMap<int,float> getChanges();
    QMap<int,float> sceneFusion(QMap<int,float> sceneA, QMap<int,float> sceneB,float ballance,QSet<int> usedLamps);

public slots:
    void testMidi();
    void nextScene();
    void prevScene();
    void onMusic();

private slots:
    void jumpClicked();
    void playOverlay();
    void playOverlayBtn();
    void requestNextOnMusic(int state);
    void requestOverlayOnMusic(int state);

private:
    Ui::MainWindow *ui;
    JackProcessor *p;
    QList<Scene*> scenes;
    QList<OverlayScene*> overlays;
    QSet<int> usedLamps;
    QMap<int,float> status;
    int currentScene;
    int currentOverlay;
    bool offsetRequested;
    long fadeStartMs,fadeStartS;
    int fading;
    void resetFadeStart();
    float getTimeSinceFadePercentage(int duration);
    void updateSceneLables();
    void stopCurrentOverlay();
    bool nextOnMusic;
    bool overlayOnMusic;
};

#endif // MAINWINDOW_H
