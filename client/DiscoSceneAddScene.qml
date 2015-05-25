import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import WebSocketConnector 1.1

Item{
    signal sceneSelected(string type, string name);
    ListModel{
        id: sceneTypeModel
        ListElement{
            text:qsTr("BeatScene")
            type: "beatScene1"
        }
        ListElement{
            text:qsTr("FlashScene")
            type:"falshScene"
        }
        ListElement{
            text:qsTr("ColorScene")
            type:"blackscene"
        }
        ListElement{
            text:qsTr("Color Wheel Scene")
            type:"colorWheel"
        }
        ListElement{
            text:qsTr("Color Wave Scene")
            type:"colorWaveScene"
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
        requestType: "discoScene"
    }
}

