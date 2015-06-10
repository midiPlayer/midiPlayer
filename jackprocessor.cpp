#include <QtGui/QMessageBox>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <stdio.h>
#include "jackprocessor.h"
#include <QDebug>
#include "mainwindow.h"
#include <QMutexLocker>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>               /* for isfinite */
#include <string.h>             /* for strcmp */
#include "device.h"
#include "websocketserver.h"

//#define DISSABLE_ANALYSE

JackProcessor::JackProcessor(WebSocketServer *ws, QObject *parent) : WebSocketServerProvider(ws), QObject(parent), musicNotificationRequested(false),
    beatRequested(true),pos(0),buffer_size(512),hop_size(256),minLevel(-80.0f) {
    ws->registerProvider(this);
}

JackProcessor::~JackProcessor() {

  //jack_port_unregister (jackHandle, jackMidi);
  jack_port_unregister (jackHandle, jackMidiOut);
  jack_client_close (jackHandle);
  del_fvec(smpl);
  del_fvec(onset);
  del_fvec(tempo_out);
  del_aubio_onset(o);
  del_aubio_tempo(tempo);

  //delete[] ibuf;

}

int JackProcessor::initJack(MainWindow* m) {

    //aubio
    smpl = new_fvec(hop_size);
    ibuf = new jack_sample_t[1025];
    tempo_out = new_fvec(2);

    mainW = m;
  if ((jackHandle = jack_client_open("MidiPlayer", JackNullOption, NULL)) == 0) {
    QMessageBox::critical(0, "MidiPlayer", "JACK server not running ?");
    exit(1);
  }
  //jackMidi = jack_port_register(jackHandle, "Scene Recorder_MIDI_In", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
  jackMidiOut = jack_port_register(jackHandle, "Playback", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
  jack_set_process_callback(jackHandle, jack_static_callback, (void *)this);
  if (jack_activate(jackHandle)) {
    QMessageBox::critical(0, "Scene Recorder", "Can't activate JACK.");
    exit(1);
  }  

  jackAudioIn = jack_port_register(jackHandle,"Audio Trigger in",JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput,0);

  samplerate = jack_get_sample_rate (jackHandle);
  o = new_aubio_onset ("default", buffer_size, hop_size, samplerate);
  aubio_onset_set_threshold (o, 0.6);
  onset = new_fvec (1);

  tempo = new_aubio_tempo("default", buffer_size, hop_size, samplerate);

  return(0);
}

void JackProcessor::clientRegistered(QJsonObject msg, int id)
{
    QJsonObject reply;
    reply.insert("minLevel",minLevel);
    sendMsg(reply,id);
}

void JackProcessor::clientUnregistered(QJsonObject msg, int clientIdCounter)
{
}

void JackProcessor::clientMessage(QJsonObject msg, int clientId)
{
    if(msg.contains("minLevel"))
        minLevel = msg.value("minLevel").toDouble(-80.0);
    sendMsgButNotTo(msg,clientId);
}

QString JackProcessor::getRequestType()
{
    return "JackProcessor";
}

QJsonObject JackProcessor::serialize()
{

}

void JackProcessor::loadSerialized()
{

}

void JackProcessor::requestMusicNotification()
{
    musicNotificationRequested = true;
}

int JackProcessor::jack_static_callback(jack_nframes_t nframes, void *arg)
{
  return ((JackProcessor *) arg)->jack_callback(nframes);
}

int JackProcessor::jack_callback(jack_nframes_t nframes)
{
  jack_midi_event_t ev;
  jack_nframes_t ev_count;
  int type, ch, index, val;

  /*void* midiBuf = jack_port_get_buffer(jackMidi, nframes);
  ev_count = jack_midi_get_event_count(midiBuf); 
  while (ev_count) {
    ev_count--;
    jack_midi_event_get(&ev, midiBuf, ev_count);
    type = *(ev.buffer) & 0xf0;
    ch = *(ev.buffer) & 0x0f;  
    index = *(ev.buffer + 1);  
    val = *(ev.buffer + 2);    
    emit midiEvent(type, ch, index, val);
  }*/

#ifndef DISSABLE_ANALYSE
    ibuf = (jack_sample_t *)jack_port_get_buffer (jackAudioIn, nframes);

    for (int j=0;j<(unsigned)nframes;j++) {
         fvec_set_sample(smpl, ibuf[j], pos);
         if (pos == (int)(hop_size) - 1) {
               float silence = aubio_silence_detection(smpl, minLevel);
               //calback:
               aubio_onset_do (o,smpl , onset); //onsets
               float res = fvec_get_sample(onset, 0);
               if(res > 0.0f && !silence){
                    //qDebug()  << res;
                    emit onsetNotification();
                    qDebug() << "lib found onset";
               }
               aubio_tempo_do (tempo, smpl, tempo_out); //beat tracking
               float beat = fvec_get_sample (tempo_out, 0);
               if(beat != 0.0f && !silence){
                   emit beatNotification();
                   qDebug() << "lib found beat";
               }

               //end Callback
               pos = -1;
         }
         pos++;
    }

/*
  //playback
    void* buffer = jack_port_get_buffer(jackMidiOut, nframes);
    jack_midi_clear_buffer(buffer);

    static int count = 0;
    if(count++ > 0)
        count = 0;
    else return 0;
    queue.clear();
    */
    /*QList<Device> devices = mainW->getChanges();
    if(devices.length() != 0)
        queue = Device::toLagacy(devices);*/


/*

    unsigned char* buffer2;
    if(queue.size() != 0){


    QMutableMapIterator<int, float> i(queue);
       while (i.hasNext()) {
           i.next();
           buffer2 = jack_midi_event_reserve(buffer, 10, 3);
           if(buffer2 == 0)
               qDebug() << "output failed";
           else{
               buffer2[0] = 190;
               int key = i.key();
               buffer2[1] = key;
               buffer2[2] = (i.value()*127.0f);
               //buffer2[2] = 100;
               //buffer2[3] = 10;
               qDebug() << "index: " << key  << "value:" << i.value()*127.0f;
               i.remove();
           }
       }
    }
*/


       jack_default_audio_sample_t *sample= (jack_default_audio_sample_t *)(jack_port_get_buffer(jackAudioIn, nframes));
           if (musicNotificationRequested && nframes > 0 && *sample != 0) {
             //emit chunkReady();
               qDebug() << "music!";
               musicNotificationRequested = false;
               emit musicNotification();
           }

#endif

/*

           if(nframes > 0){
               double sum;
               for(int i = 0; i < nframes;i++){
                   float s = *(sample++);
                   if(s < 0)
                        s *= -1;
                    sum += s;
               }

               float s = sum / (double)nframes;
               static float max = 0.0;
               static float av = 0;
               av = (9*av + s) / 10;
              // qDebug() << av;
               //if(s > av*1.7){
              // qDebug() << max;
               static int speedLimit = 0;
               if(s > max * 1.1 && s > av*1.2 && s > 0.1 && speedLimit < 0){
                    max = s;
                    qDebug() << "beat";
                    emit beatNotification();
                    speedLimit = 7;
               }
               speedLimit --;
               max -= 0.01;
               max *= 0.99;
               if(max < 0)
                   max = 0;
           }*/
  return 0;
}
