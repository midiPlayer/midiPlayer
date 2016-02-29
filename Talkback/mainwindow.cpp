#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVariant>
#include <QDebug>

#define KEY_SETTINGS_INPUT_DEV "input_dev"
#define KEY_SETTINGS_OUTPUT_DEV "output_dev"
#define KEY_SETTINGS_VOL "volume"

MainWindow::MainWindow(bool playJingle, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), startProc(this), stopProc(this), settings(), mPlayer(this)
{
    QCoreApplication::setOrganizationName("fdg");
    QCoreApplication::setOrganizationDomain("fdg-ab.de");
    QCoreApplication::setApplicationName("light master");

    ui->setupUi(this);

    connect(ui->doneBtn,SIGNAL(clicked()),this,SLOT(done()));
    ui->inputDevEdit->setText(settings.value(KEY_SETTINGS_INPUT_DEV).toString());
    ui->outputDevEdit->setText(settings.value(KEY_SETTINGS_OUTPUT_DEV).toString());
    ui->volumeSlider->setValue(settings.value(KEY_SETTINGS_VOL).toDouble() * 100);

    if(playJingle){
        mPlayer.setVolume(settings.value(KEY_SETTINGS_VOL).toDouble() * 100);
        mPlayer.setMedia(QUrl("qrc:/sbbJingle.ogg"));
        mPlayer.play();
    }

    start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::done()
{
    settings.setValue(KEY_SETTINGS_INPUT_DEV,ui->inputDevEdit->text());
    settings.setValue(KEY_SETTINGS_OUTPUT_DEV,ui->outputDevEdit->text());
    settings.setValue(KEY_SETTINGS_VOL,ui->volumeSlider->value() / 100.0);
    stop();
}

void MainWindow::close()
{
    exit(0);
}

void MainWindow::stop()
{
    connect(&stopProc,SIGNAL(finished(int)),this,SLOT(close()));
    connect(&stopProc,SIGNAL(error(QProcess::ProcessError)),this,SLOT(close()));
    QStringList args;
    args << settings.value(KEY_SETTINGS_INPUT_DEV).toString();
    args << settings.value(KEY_SETTINGS_OUTPUT_DEV).toString();
    stopProc.start("jack_disconnect",args);
}

void MainWindow::start()
{
    qDebug() << "started";
    QStringList args;
    args << settings.value(KEY_SETTINGS_INPUT_DEV).toString();
    args << settings.value(KEY_SETTINGS_OUTPUT_DEV).toString();
    startProc.start("jack_connect",args);
}
