import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import WebSocketConnector 1.1

Item {
    ColumnLayout{
        width:300
        height: 300
        anchors.centerIn: parent
        spacing: 10
        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            RowLayout{
                spacing: 10
                anchors.fill: parent
                Text{
                    color:"#fff"
                    text: qsTr("min Level")
                }
                Slider{
                    id: minLevelSlider
                    Layout.fillWidth: true
                    minimumValue: -100
                    maximumValue: 0
                    onValueChanged: {
                        var msg = {"minLevel":value};
                        ws.send = JSON.stringify(msg)
                    }
                }
            }
        }
        Item{
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            TriggerSourceBtn{

            }
        }
    }
    WebSocketConnector{
        id:ws
        requestType: "JackProcessor"
        onMessage: {
            console.log(JSON.stringify(msg))
            if(msg.minLevel !== undefined)
                minLevelSlider.value = msg.minLevel;
        }
    }
}

