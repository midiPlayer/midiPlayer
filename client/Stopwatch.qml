import QtQuick 2.0
import WebSocketConnector 1.1
Item {
    property var referenzeTime;
    property int snappedTime : 0;
    property bool running : false;

    property alias requestId : ws.requestId

    signal dataChanged();

    Timer{
        id:timer
        running: false
        repeat: true
        interval: 1000/30;
        onTriggered: {
            dataChanged()
        }


    }

    function start(notify){
        referenzeTime = Date.now();
        running = true;

        if(notify){
            var msg = new Object();
            msg.start = true;
            ws.send = JSON.stringify(msg);
        }

        timer.start();
    }

    function setTime(time,notify){
        snappedTime = time;
        referenzeTime = Date.now() - time;
        if(notify){
            var msg = new Object();
            msg.set = getTime();
            ws.send = JSON.stringify(msg);
        }
    }

    function resume(notify){
        setTime(snappedTime,false);
        running = true;

        if(notify){
            var msg = new Object();
            msg.resume = true;
            ws.send = JSON.stringify(msg);
        }

        timer.start();
    }

    function getTime(){
        if(!running)
            return snappedTime;
        return Date.now() - referenzeTime;
    }

    function stop(notify){
        snappedTime = getTime();
        running = false;

        if(notify){
            var msg = new Object();
            msg.stop = true;
            ws.send = JSON.stringify(msg);
        }

        timer.stop();
    }


    WebSocketConnector{
        id:ws
        onMessage:  {
            console.log(JSON.stringify(msg));
            if(msg.hasOwnProperty("start")){
                start(false);
                dataChanged();
            }
            if(msg.hasOwnProperty("stop")){
                stop(false);
                dataChanged();
            }
            if(msg.hasOwnProperty("set")){
                setTime(msg.set,false);
                dataChanged();
            }
            if(msg.hasOwnProperty("resume")){
                resume(false);
                dataChanged();
            }

        }
    }

}
