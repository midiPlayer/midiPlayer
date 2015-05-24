#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include "scene.h"
#include "overlayscene.h"
#include "jackprocessor.h"
#include "device.h"
#include "websocketserver.h"
#include "beamerdeviceprovider.h"
#include "oladeviceprovider.h"
#include <QList>
#include "outputdevice.h"
#include <QTimer>
#include "remotebeat.h"
#include "discoscene.h"
#include <QSharedPointer>
#include "beamershutterscenemanager.h"
#include "scenebuilder.h"
#include <QSettings>
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getChanges();

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
    void trigger();

private:
    Ui::MainWindow *ui;
    JackProcessor jackProcessor;
    QList<QSharedPointer<Scene> > scenes;
    QList<OverlayScene*> overlays;
    QList<Device> usedLamps;
    QList<Device> status;
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
    QList<Device> availableDevices;
    WebSocketServer wss;
    QList<OutputDevice*> outDevices;
    bool getChangesRunning;
    QTimer timer;
    RemoteBeat remoteBeat;
    QSharedPointer<DiscoScene> discoscene;
    BeamerShutterSceneManager beamerShutterSceneManager;
    beamerDeviceProvider myBeamerDeviceProvider;
    OlaDeviceProvider olaDeviceProvider;
    SceneBuilder sceneBuilder;
    QSettings settings;
    QJsonObject getDiscoScenSettings();

};

#endif // MAINWINDOW_H
