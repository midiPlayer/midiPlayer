import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
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
                GridLayout{
                    width: parent.width
                    columns: 2
                    rowSpacing: 20
                    Text{
                        text:qsTr("Foreground trigger:")
                        color:"#fff";
                    }
                    TriggerSourceBtn{
                        id:foregroundBtn
                    }
                        Text{
                            text:qsTr("Background trigger:")
                            color:"#fff";
                        }
                        TriggerSourceBtn{
                            id:backroundBtn
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
                spacing: 20

                ColorPickerButton{
                    id: colorPickerButton
                    minColorNum: 3
                }

                SelectVirtualDevieManagerHook{
                    id:vDevManager
                }
            }

    }


    WebSocketConnector{
        id: wsc
        onMessage: {
            if(msg.config !== undefined){
                if(msg.config.foregroundTrigger !== undefined)
                    foregroundBtn.requestId = msg.config.foregroundTrigger;
                if(msg.config.backgroundTrigger !== undefined)
                    backroundBtn.requestId = msg.config.backgroundTrigger;
                if(msg.config.smoothnessChanged !== undefined)
                    smoothnessSlider.value  = msg.config.smoothnessChanged;
                if(msg.config.colorButton!==undefined)
                    colorPickerButton.requestId = msg.config.colorButton;
                if(msg.config.hasOwnProperty("selectDevManager"))
                    vDevManager.requestId = msg.config.selectDevManager;

            }
            if(msg.smoothnessChanged !== undefined)
                smoothnessSlider.value  = msg.smoothnessChanged;
        }
    }

}
