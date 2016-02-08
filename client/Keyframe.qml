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
            msg.state = [];
            if(value.hasR)
                msg.state.push(value.r);
            if(value.hasG)
                msg.state.push(value.g);
            if(value.hasB)
                msg.state.push(value.b);
            if(value.hasW)
                msg.state.push(value.w);
            ws.send = JSON.stringify(msg);
        });
    }

    function setLiveEditing(editing) {
        var msg = new Object();
        msg.liveEditing = editing;
        ws.send = JSON.stringify(msg)
    }

    WebSocketConnector{
        id: ws
        onMessage: {
            console.log(JSON.stringify(msg));
            if(msg.hasOwnProperty("time")){
                time = msg.time;
            }
            if(msg.hasOwnProperty("state")){
                var i = 0;
                if(value.hasR && msg.state.length > i) value.passivR = msg.state[i++];
                if(value.hasG && msg.state.length > i) value.passivG = msg.state[i++];
                if(value.hasB && msg.state.length > i) value.passivB = msg.state[i++];
                if(value.hasW && msg.state.length > i) value.passivW = msg.state[i++];
            }
            if(msg.hasOwnProperty("deleteKeyframe")){
                keyframe.deleteRequested();
            }
        }
    }
}


