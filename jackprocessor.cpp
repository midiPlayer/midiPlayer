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

JackProcessor::JackProcessor(QObject *parent) : QObject(parent), musicNotificationRequested(false),beatRequested(true) {
}

JackProcessor::~JackProcessor() {

  //jack_port_unregister (jackHandle, jackMidi);
  jack_port_unregister (jackHandle, jackMidiOut);
  jack_client_close (jackHandle);
}

int JackProcessor::initJack(MainWindow* m) {
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

  //aubio
  smpl = new_fvec(hop_size);
  ibuf = new jack_sample_t[1025];

  o = new_aubio_onset ("hfc", buffer_size, hop_size, samplerate);
  samplerate = jack_get_sample_rate (jackHandle);
  onset = new_fvec (1);
  aubio_onset_set_threshold (o, 0.6);

  return(0);
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

    ibuf = (jack_sample_t *)jack_port_get_buffer (jackAudioIn, nframes);
/*
    for (int j=0;j<(unsigned)nframes;j++) {
         fvec_set_sample(smpl, ibuf[j], pos);
         if (pos == (int)(hop_size) - 1) {
               //calback:
               aubio_onset_do (o,smpl , onset);
               float res = fvec_get_sample(onset, 0);
               if(res > 0.0f){
                    qDebug()  << res;
                    emit beatNotification(res);
                   qDebug() << "lib found beat";
               }
               //end Callback
               pos = -1;
         }
         pos++;
    }*/

  //playback
    void* buffer = jack_port_get_buffer(jackMidiOut, nframes);
    jack_midi_clear_buffer(buffer);

    static int count = 0;
    if(count++ > 0)
        count = 0;
    else return 0;
    queue.clear();
    /*QList<Device> devices = mainW->getChanges();
    if(devices.length() != 0)
        queue = Device::toLagacy(devices);*/




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



       jack_default_audio_sample_t *sample= (jack_default_audio_sample_t *)(jack_port_get_buffer(jackAudioIn, nframes));
           if (musicNotificationRequested && nframes > 0 && *sample != 0) {
             //emit chunkReady();
               qDebug() << "music!";
               musicNotificationRequested = false;
               emit musicNotification();
           }



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
