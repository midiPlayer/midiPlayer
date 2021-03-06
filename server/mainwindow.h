#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QList>
#include <QMap>
#include "scene.h"
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
#include "diascene.h"
#include "fileioprovider.h"
#include "monitorio.h"
#include "hardcodedvirtualdevicemanager.h"

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    void getChanges();

    void loadScenes(QJsonObject data);
public slots:
    void trigger();
    void save();

private:
    QMap<QString, QSharedPointer<DeviceState> > status;

    bool offsetRequested;

    void resetFadeStart();
    float getTimeSinceFadePercentage(int duration);

    HardcodedVirtualDeviceManager virtualDevManager;

    WebSocketServer wss;
    JackProcessor jackProcessor;
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
    QJsonObject getMainScenSettings();

    QSharedPointer<DiaScene> mainScene;
    FileIOProvider filieIoProv;

    MonitorIO monitorIO;
};

#endif // MAINWINDOW_H
