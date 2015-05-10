import QtQuick 2.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1

Item{
    width: parent.width;
    height:200
    id:main

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20

            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Foreground trigger:")
                    color:"#fff";
                }
                TriggerSourceBtn{
                    id:foregroundBtn
                    onStateChanged: {
                        var msg = new Object();
                        msg.foregroundTrigger = new Object();
                        msg.foregroundTrigger.onset = onset;
                        msg.foregroundTrigger.beat = beat;
                        msg.foregroundTrigger.timer = timer;
                        wsc.send = JSON.stringify(msg);
                    }
                }
            }
            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Background trigger:")
                    color:"#fff";
                }
                TriggerSourceBtn{
                    id:backroundBtn
                    onStateChanged: {
                                            var msg = new Object();
                                            msg.backgroundTrigger = new Object();
                                            msg.backgroundTrigger.onset = onset;
                                            msg.backgroundTrigger.beat = beat;
                                            msg.backgroundTrigger.timer = timer;
                                            wsc.send = JSON.stringify(msg);
                                        }
                }
                }

    }


    WebSocketConnector{
        id: wsc
        requestType: "beatScene1"
        onMessage: {
            if(msg.foregroundTrigger !== undefined)
                foregroundBtn.setState(msg.foregroundTrigger.onset,msg.foregroundTrigger.beat,msg.foregroundTrigger.timer);
            if(msg.backgroundTrigger !== undefined)
                backroundBtn.setState(msg.backgroundTrigger.onset,msg.backgroundTrigger.beat,msg.backgroundTrigger.timer);
        }
    }

}
