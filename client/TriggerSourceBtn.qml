import QtQuick 2.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1

    Rectangle{
        property bool radio : true
        property alias requestId : stateWsc.requestId
        signal stateChanged(bool onset,bool beat,bool timer);
        signal modalStateChanged(string trigger);

        onStateChanged: {
            var msg = new Object();
            msg.setTrigger = new Object();
            msg.setTrigger.onset = onset;
            msg.setTrigger.beat = beat;
            msg.setTrigger.timer = timer;
            stateWsc.send = JSON.stringify(msg);
        }

        function setState(onset,beat,timer){
            beatBtn.isOn = beat;
            onsetBtn.isOn = onset;
            timerBtn.isOn = timer;
        }

        id: box
        height:30
        width: 180
        radius: 8
        color:'black'
        RowLayout{
            anchors.fill: parent
            spacing: 1
            TriggerSourceBtnItem{
                id: beatBtn
                isFirst: true
                text:"beat"
                width: parent.width / 3;
                cornerRadius: box.radius;
                onStateOn: {
                    if(radio){
                        onsetBtn.isOn = false;
                        timerBtn.isOn = false;
                    }
                    sendState();
                }
                onStateOff: sendState();
            }
            TriggerSourceBtnItem{
                id: onsetBtn
                text:"onset"
                width: parent.width / 3;
                onStateOn: {
                    if(radio){
                        beatBtn.isOn = false;
                        timerBtn.isOn = false;
                    }
                    sendState();
                }
                onStateOff: sendState();
            }
            TriggerSourceBtnItem{
                id: timerBtn
                text:"timer"
                width: parent.width / 3;
                isLast: true
                cornerRadius: box.radius
                onStateOn: {
                    if(radio){
                        onsetBtn.isOn = false;
                        beatBtn.isOn = false;
                    }
                    sendState();
                }
                onStateOff: sendState();
            }
        }

        function sendState(){
            stateChanged(onsetBtn.isOn,beatBtn.isOn,timerBtn.isOn);
            console.log(beatBtn.isOn);
            var method = "";
            if(onsetBtn.isOn)
                method = "onset";
            else if(beatBtn.isOn)
                method = "beat";
            else if(timerBtn.isOn)
                method = "timer";
            modalStateChanged(method);
        }



WebSocketConnector{
    requestType:"remoteBeat"
    onMessage: {
        if(msg.type === "beat")
            beatBtn.trigger();
        if(msg.type === "onset")
            onsetBtn.trigger();

    }
}
WebSocketConnector{
    id:stateWsc
    onMessage: {
        if(msg.setTrigger !== undefined)
            setState(msg.setTrigger.onset,msg.setTrigger.beat,msg.setTrigger.timer);
    }
}

}

