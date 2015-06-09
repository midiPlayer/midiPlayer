import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import WebSocketConnector 1.1

Item{
    property alias requestId: ws.requestId
    property alias requestType: ws.requestType
    ListModel{
        id: sceneTypeModel
        ListElement{
            text:qsTr("BeatScene")
            type: "beatScene1"
        }
        ListElement{
            text:qsTr("FlashScene")
            type:"flashScene"
        }
        ListElement{
            text:qsTr("ColorScene")
            type:"colorScene"
        }
        ListElement{
            text:qsTr("Color Wheel Scene")
            type:"colorWheel"
        }
        ListElement{
            text:qsTr("Color Wave Scene")
            type:"colorWaveScene"
        }
        ListElement{
            text: qsTr("alternating Beat Scene")
            type:"beatScene2"
        }
        ListElement{
            text: qsTr("Disco Scene")
            type:"discoScene"
        }
    }

    ColumnLayout{
        //width:600;
        spacing: 20
        anchors.centerIn: parent;
        RowLayout{
            Layout.fillWidth: true;
            ComboBox{
                id: sceneTypeComo
                model: sceneTypeModel
            }
        }
        RowLayout{
            Layout.fillWidth: true;
            Text{
                text:qsTr("scene name")
                color:"#fff";
            }
            TextField{
                id:nameEdit
            }
        }
        RowLayout{
            spacing: 10
            Button{
                text: qsTr("add")
                onClicked: {
                    var msg = new Object();
                    msg.addScene = new Object();
                    msg.addScene.name = nameEdit.text;
                    msg.addScene.type = sceneTypeModel.get(sceneTypeComo.currentIndex).type;
                    ws.send = JSON.stringify(msg);
                    stackView.pop();
                }
            }
        }
    }
    WebSocketConnector{
        id: ws
    }
}

