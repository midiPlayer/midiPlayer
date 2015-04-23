#ifndef JACKPROCESSOR_H
#define JACKPROCESSOR_H

#include <QtCore/QObject>
#include <jack/jack.h>
#include <QMap>

class MainWindow;
class JackProcessor : public QObject
{
  Q_OBJECT

  private:
    jack_client_t *jackHandle;
    jack_port_t *jackMidi;
    QMap<int,float> queue;
    MainWindow* mainW;

  private:
    static int jack_static_callback(jack_nframes_t nframes, void *arg);
    int jack_callback(jack_nframes_t nframes);

  public:
    JackProcessor(QObject *parent=0);
    ~JackProcessor();
    int initJack(MainWindow *w);
    void pushCommand(int lamp,float state);
signals:
    void midiRequest();
};

#endif
