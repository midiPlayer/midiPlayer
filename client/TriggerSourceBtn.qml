import QtQuick 2.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1

    Rectangle{
        property bool radio : true
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
                }
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
                }
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
                }
            }
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

}

