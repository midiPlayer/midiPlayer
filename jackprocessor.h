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
    //jack_port_t *jackMidi;
    jack_port_t *jackMidiOut;
    jack_port_t *jackAudioIn;
    QMap<int,float> queue;
    MainWindow* mainW;
    
  private:
    static int jack_static_callback(jack_nframes_t nframes, void *arg);
    int jack_callback(jack_nframes_t nframes);
    bool musicNotificationRequested;
    bool beatRequested;

  public:
    JackProcessor(QObject* parent=0);
    ~JackProcessor();
    int initJack(MainWindow *m);
public slots:
    void requestMusicNotification();

  signals:
    //void midiEvent(int type, int ch, int index, int val);
    void musicNotification();
    void beatNotification();
};
  
#endif
