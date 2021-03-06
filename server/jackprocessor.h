#ifndef JACKPROCESSOR_H
#define JACKPROCESSOR_H

#include <QtCore/QObject>
#include <jack/jack.h>
#include <QMap>
#include <aubio/aubio.h>
#include "websocketserverprovider.h"

class MainWindow;

class JackProcessor : public QObject, public WebSocketServerProvider
{
  Q_OBJECT

    typedef jack_default_audio_sample_t jack_sample_t;
  
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
    jack_sample_t *ibuf;
    aubio_onset_t *o;
    aubio_tempo_t * tempo;
    fvec_t * tempo_out;
    int samplerate;
    uint_t buffer_size;
    uint_t hop_size;
    fvec_t *onset;
    /** internal fvec */
     fvec_t *smpl;
     int pos;
     float minLevel;

     void loadSerialized(QJsonObject serialized);
  public:
    JackProcessor(WebSocketServer *ws, QObject* parent=0);
    ~JackProcessor();
    int initJack(MainWindow *m);

    void clientRegistered(QJsonObject msg, int id);
    void clientUnregistered(QJsonObject msg,int clientIdCounter);
    void clientMessage(QJsonObject msg, int clientId);
    QString getRequestType();
    QJsonObject serialize();
public slots:
    void requestMusicNotification();

  signals:
    //void midiEvent(int type, int ch, int index, int val);
    void musicNotification();
    void beatNotification();
    void onsetNotification();
};
  
#endif
