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
                            text:qsTr("Background trigger:")
                            color:"#fff";
                        }
                        TriggerSourceBtn{
                            id:backroundBtn
                        }
                        Text {
                            text: qsTr("changing Devices")
                            color:"#fff";
                        }
                        RowLayout{
                            Slider{
                                id:numChangingDevSlider
                                Layout.fillWidth: true;
                                stepSize: 1
                                minimumValue: 0
                                maximumValue: 20

                                onValueChanged: {
                                    var msg = {"changingDevices":value};
                                    wsc.send = JSON.stringify(msg);
                                }
                            }
                            Text{
                                color:"#fff";
                                text: numChangingDevSlider.value == 0 ? qsTr("all") : numChangingDevSlider.value
                                Layout.minimumWidth: 20
                            }
                        }

                        Text{
                            text:qsTr("Smoothness")
                            color:"#fff";
                        }
                        Slider{
                            id: smoothnessSlider
                            minimumValue: 0
                            maximumValue: 0.5
                            Layout.fillWidth: true;
                            onValueChanged: {
                                if(pressed){
                                    var mesage = new Object();
                                    mesage.smoothnessChanged = value;
                                    wsc.send = JSON.stringify(mesage);
                                }
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

                CheckBox{
                    id: sameColorCb
                    text:qsTr("Same Color");
                    onCheckedChanged: {
                        var msg = {"sameColor":checked};
                        wsc.send = JSON.stringify(msg);
                    }
                }
            }

    }


    WebSocketConnector{
        id: wsc
        onMessage: {
            console.log(JSON.stringify(msg));
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
                if(msg.config.hasOwnProperty("numDevs"))
                    numChangingDevSlider.maximumValue = msg.config.numDevs;
                if(msg.config.hasOwnProperty("changingDevices"))
                    numChangingDevSlider.value = msg.config.changingDevices;
                if(msg.config.hasOwnProperty("sameColor"))
                    sameColorCb.checked = msg.config.sameColor;

            }
            if(msg.smoothnessChanged !== undefined)
                smoothnessSlider.value  = msg.smoothnessChanged;
        }
    }

}
