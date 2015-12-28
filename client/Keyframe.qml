import QtQuick 2.0
import RGBWColor 1.1
import WebSocketConnector 1.1

Item{
    id: keyframe
    RGBWColor{
        id:zeroColor
    }
    property RGBWColor value: zeroColor
    property double time: 0
    property alias requestId : ws.requestId;

    signal deleteRequested();

    function setTime(t){
        time = t;
        var msg = new Object();
        msg.time = time;
        ws.send = JSON.stringify(msg);
    }

    function requestDelete() {
        var msg = new Object();
        msg.deleteKeyframe = true;
        ws.send = JSON.stringify(msg);
    }


    Component.onCompleted: {
        value.onColorActiveChanged.connect(function(){
            var msg = new Object();
            msg.state = [value.r, value.g, value.b, value.w];
            ws.send = JSON.stringify(msg);
        });
    }


    WebSocketConnector{
        id: ws
        onMessage: {
            console.log(JSON.stringify(msg));
            if(msg.hasOwnProperty("time")){
                time = msg.time;
            }
            if(msg.hasOwnProperty("state") && msg.state.length === 4){
                value.passivR = msg.state[0];
                value.passivG = msg.state[1];
                value.passivB = msg.state[2];
                value.passivW = msg.state[3];
            }
            if(msg.hasOwnProperty("deleteKeyframe")){
                keyframe.deleteRequested();
            }
        }
    }
}


