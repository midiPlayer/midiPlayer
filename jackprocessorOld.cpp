#include <QtGui/QMessageBox>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <stdio.h>
#include "jackprocessor.h"
#include <QDebug>
#include <QMapIterator>
#include "mainwindow.h"


JackProcessor::JackProcessor(QObject *parent) : QObject(parent),queue() {
}

JackProcessor::~JackProcessor() {

  jack_port_unregister (jackHandle, jackMidi);
  jack_client_close (jackHandle);
}

int JackProcessor::initJack(MainWindow *w) {
    mainW = w;

  if ((jackHandle = jack_client_open("MidiGenerator", JackNullOption, NULL)) == 0) {
    QMessageBox::critical(0, "QMidiViewer", "JACK server not running ?");
    exit(1);
  }
  // create MIDI in/out ports
  jackMidi = jack_port_register(jackHandle, "Midi Generator", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
  jack_set_process_callback(jackHandle, jack_static_callback, (void *)this);
  if (jack_activate(jackHandle)) {
    QMessageBox::critical(0, "QMidiViewer", "Can't activate JACK.");
    exit(1);
  }
  return(0);
}

void JackProcessor::pushCommand(int lamp, float state){
    if(queue.contains(lamp))
        queue.remove(lamp);//alte stati löschen
    queue.insert(lamp,state);
}






int JackProcessor::jack_static_callback(jack_nframes_t nframes, void *arg)
{
    /* Check for impossible condition that actually happened to me, caused by some problem between jackd and OSS4. */
        if (nframes <= 0) {
            qDebug() << ("Process callback called with nframes = 0; bug in JACK?");
            return 0;
        }
  return ((JackProcessor *) arg)->jack_callback(nframes);
}

int JackProcessor::jack_callback(jack_nframes_t nframes)
{
    void* buffer = jack_port_get_buffer(jackMidi, nframes);
    jack_midi_clear_buffer(buffer);

    static int count = 0;
    if(count++ > 2)
        count = 0;
    else return 0;
    mainW->getChanges();
 /* jack_midi_event_t ev;
  jack_nframes_t ev_count;
  int type, ch, index, val;

  void* midiBuf = jack_port_get_buffer(jackMidi, nframes);
  ev_count = jack_midi_get_event_count(midiBuf);
  while (ev_count) {
    ev_count--;
    jack_midi_event_get(&ev, midiBuf, ev_count);
    type = *(ev.buffer) & 0xf0;
    ch = *(ev.buffer) & 0x0f;
    index = *(ev.buffer + 1);
    val = *(ev.buffer + 2);
  }*/

 /* int data_size;
 // the application specific part, assuming this function
 // returns a buffer with standard MIDI data, and sets
 // "data_size" to the size of the buffer in bytes
 char* data = getMidiData(&data_size);
 // copy the app generated MIDI data to the output port
 void* buffer = jack_port_get_buffer(JackProcessor::jackMidi, nframes);
 jack_midi_clear_buffer(buffer);
 jack_midi_data_t* midi_data_buffer = jack_midi_event_reserve(buffer, 0, data_size);
 memcpy(midi_data_buffer, data, data_size);*/


 unsigned char* buffer2;
 if(queue.size() == 0){
     return 0;
 }

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
            buffer2[2] = (int)(i.value()*127.0f);
            qDebug() << "index: " << key  << "value:" << i.value()*127.0f;
            i.remove();
        }
    }
 return 0;
}

