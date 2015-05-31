import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.0
import WebSocketConnector 1.1

Item{
    width: parent.width;
    height:200
    id:main
    property alias requestId : wsc.requestId

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20

            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Trigger:")
                    color:"#fff";
                }
                TriggerSourceBtn{
                    id:triggerBtn
                }
            }
            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Duration")
                    color:"#fff";
                }
                Slider{
                    id: durationSlider
                    minimumValue: 0.1
                    maximumValue: 3
                    onValueChanged: {
                        if(pressed){
                            var mesage = new Object();
                            mesage.durationChanged = value;
                            wsc.send = JSON.stringify(mesage);
                        }
                    }
                }

            }
            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Basic Speed")
                    color:"#fff";
                }
                Slider{
                    id: basicSpeedSlider
                    minimumValue: 0
                    maximumValue: 1
                    onValueChanged: {
                        if(pressed){
                            var mesage = new Object();
                            mesage.basicSpeedChanged = value;
                            wsc.send = JSON.stringify(mesage);
                        }
                    }
                }
            }
            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Bump Speed")
                    color:"#fff";
                }
                Slider{
                    id: bumpSpeedSlider
                    minimumValue: 0
                    maximumValue: 3
                    onValueChanged: {
                        if(pressed){
                            var mesage = new Object();
                            mesage.bumpSpeedChanged = value;
                            wsc.send = JSON.stringify(mesage);
                        }
                    }
                }
            }

    }


    WebSocketConnector{
        id: wsc
        onMessage: {
            console.log(JSON.stringify(msg));
           if(msg.trigger !== undefined)
            triggerBtn.requestId = msg.trigger;
           if(msg.bumpSpeedChanged !== undefined)
            bumpSpeedSlider.value  = msg.bumpSpeedChanged;
           if(msg.basicSpeedChanged !== undefined)
            basicSpeedSlider.value  = msg.basicSpeedChanged;
           if(msg.durationChanged !==undefined)
            durationSlider.value = msg.durationChanged;
        }
    }

}

