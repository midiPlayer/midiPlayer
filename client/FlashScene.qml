import QtQuick 2.0

import QtQuick 2.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1

import QtQuick.Controls 1.0

Item{
    width: parent.width;
    height:250
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
                    minimumValue: 1
                    maximumValue: 500
                    Layout.preferredWidth: 200
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
                    text:qsTr("Smoothness")
                    color:"#fff";
                }
                Slider{
                    id: smoothnessSlider
                    minimumValue: 0
                    maximumValue: 0.5
                    Layout.preferredWidth: 200
                    onValueChanged: {
                        if(pressed){
                            var mesage = new Object();
                            mesage.smoothnessChanged = value;
                            wsc.send = JSON.stringify(mesage);
                        }
                    }
                }
            }
            RowLayout{
                width: parent.width
                Text{
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text:qsTr("Color")
                    color:"#fff";
                }
                ColorPickerButton{
                    id: colorPickerButton
                    minColorNum: 1
                    onlyOne: true
                }
            }
    }


    WebSocketConnector{
        id: wsc
        onMessage: {
                    console.log(JSON.stringify(msg));
            if(msg.trigger !== undefined)
                triggerBtn.requestId = msg.trigger;
            if(msg.smoothnessChanged !== undefined)
                smoothnessSlider.value  = msg.smoothnessChanged;
            if(msg.durationChanged !== undefined)
                durationSlider.value  = msg.durationChanged;
            if(msg.color !== undefined)
                colorPickerButton.requestId = msg.color;
        }
    }

}
