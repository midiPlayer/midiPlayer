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
        width: 350
        radius: 8
        color:'black'
        RowLayout{
            anchors.fill: parent
            spacing: 1
            TriggerSourceBtnItem{
                id: beatBtn
                isFirst: true
                text:qsTr("Beat")
                width: parent.width / 5;
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
                text:qsTr("Onset")
                width: parent.width / 5;
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
                text:qsTr("Timer")
                width: parent.width / 5;
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
            TriggerSourceBtnItem{
                id: manualBtn
                text: qsTr("Manual")
                width: parent.width / 5;
                isLast: false
                cornerRadius: box.radius
                onStateOn: {
                    var msg = Object();
                    msg.manual = true;
                    stateWsc.send = JSON.stringify(msg);
                }
                snapIn: false
                onStateOff: sendState();
            }
            TriggerSourceBtnItem{
                property int numBeats: 0
                id: numBeatsBtn
                text: numBeats+1
                width: parent.width / 5;
                isLast: true
                cornerRadius: box.radius
                onStateOff: {
                    numBeats += 1;
                    if(numBeats >= 10)
                        numBeats = 0;
                    var msg = Object();
                    msg.numBeats = numBeats;
                    stateWsc.send = JSON.stringify(msg);
                }


                snapIn: false
            }
        }

        function sendState(){
            stateChanged(onsetBtn.isOn,beatBtn.isOn,timerBtn.isOn);
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
        if(msg.hasOwnProperty("numBeats"))
            numBeatsBtn.numBeats = msg.numBeats;
    }
}

}

