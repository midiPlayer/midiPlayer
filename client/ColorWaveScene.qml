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
                    text:qsTr("Active radius (in m)")
                    color:"#fff";
                }
                Slider{
                    id: activeRadiusSlider
                    minimumValue: 0
                    maximumValue: 100
                    Layout.preferredWidth: 200
                    onValueChanged: {
                        if(pressed){
                            var mesage = new Object();
                            mesage.activeRadiusChanged = value;
                            wsc.send = JSON.stringify(mesage);
                        }
                    }
                }

            }
            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Speed (in m/s)")
                    color:"#fff";
                }
                Slider{
                    id: speedSlider
                    minimumValue: 1
                    maximumValue: 100
                    Layout.preferredWidth: 200
                    onValueChanged: {
                        if(pressed){
                            var mesage = new Object();
                            mesage.speedChanged = value;
                            wsc.send = JSON.stringify(mesage);
                        }
                    }
                }

            }
            RowLayout{
                width: parent.width
                ColorPickerButton{
                    id: colorPickerButton
                    minColorNum: 1
                }

                SelectVirtualDevieManagerHook{
                    id:vDevManager
                }
            }

    }


    WebSocketConnector{
        id: wsc
        onMessage: {
           if(msg.trigger !== undefined)
            triggerBtn.requestId = msg.trigger;
           if(msg.activeRadiusChanged !== undefined)
            activeRadiusSlider.value  = msg.activeRadiusChanged;
           if(msg.speedChanged !== undefined)
            speedSlider.value  = msg.speedChanged;
           if(msg.colorButton!==undefined)
            colorPickerButton.requestId = msg.colorButton;
           if(msg.hasOwnProperty("devManager"))
               vDevManager.requestId = msg.devManager;
        }
    }

}

