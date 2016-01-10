#!/bin/sh
### BEGIN INIT INFO
# Provides:          MidiPlayer
# Required-Start:    $local_fs $network $named $time $syslog
# Required-Stop:     $local_fs $network $named $time $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Description:       service script for midiPlayer
### END INIT INFO

SCRIPT="/home/jakob/workspace/qt/midiPlayer-git/build-MidiPlayer-Qt_5_5_0_System-Debug/MidiPlayer"
RUNAS="jakob"

PIDFILE=/var/run/MidiPlayer.pid
LOGFILE=/var/log/MidiPlayer.log

start() {
  if [ -f "$PIDFILE" ] && kill -0 $(cat "$PIDFILE"); then
    echo 'Service already running' >&2
    return 1
  fi
  echo 'Starting service…' >&2
  local CMD="$SCRIPT &> \"$LOGFILE\" & echo \$!"
  su -c "$CMD" $RUNAS > "$PIDFILE"
  echo 'Service started' >&2
}

stop() {
  if [ ! -f "$PIDFILE" ] || ! kill -0 $(cat "$PIDFILE"); then
    echo 'Service not running' >&2
    return 1
  fi
  echo 'Stopping service…' >&2
  kill -15 $(cat "$PIDFILE") && rm -f "$PIDFILE"
  echo 'Service stopped' >&2
}

uninstall() {
  echo -n "Ehhh no! "
}
status(){
  cat $LOGFILE
  if [ ! -f "$PIDFILE" ] || ! kill -0 $(cat "$PIDFILE"); then
    echo "Service is stopped";
  else
    echo "Service is running";
  fi
}


case "$1" in
  start)
    start
    ;;
  stop)
    stop
    ;;
  uninstall)
    uninstall
    ;;
  retart)
    stop
    start
    ;;
  status)
    status
    ;;
  *)
    echo "Usage: $0 {start|stop|restart|uninstall}"
esac
