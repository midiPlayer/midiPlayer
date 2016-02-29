#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QSettings>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool playJingle,QWidget *parent = 0);
    ~MainWindow();
public slots:
    void done();
    void close();
private:
    Ui::MainWindow *ui;
    void stop();
    void start();
    QProcess startProc;
    QProcess stopProc;
    QSettings settings;
    QMediaPlayer mPlayer;
};

#endif // MAINWINDOW_H
