import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import WebSocketConnector 1.1

Item{
    property alias requestId: ws.requestId
    property alias requestType: ws.requestType

    function chooseCorrectListModel(requestType){
        if(requestType === "diaScene"){
            return sceneTypeModelDia
        }
        else{
            return sceneTypeModelDisco
        }
    }

    ListModel{
        id: sceneTypeModelDia
        ListElement{
            text:qsTr("Color Scene")
            type:"colorScene"
        }
        ListElement{
            text: qsTr("Disco Scene")
            type:"discoScene"
        }
        ListElement{
            text: qsTr("Keyframe Scene")
            type:"keyFrameScene"
        }
    }

    ListModel{
        id: sceneTypeModelDisco
        ListElement{
            text:qsTr("Beat Scene")
            type: "beatScene1"
        }
        ListElement{
            text:qsTr("Flash Scene")
            type:"flashScene"
        }
        ListElement{
            text:qsTr("Color Scene")
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
            text: qsTr("Alternating Beat Scene")
            type:"beatScene2"
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
                model:chooseCorrectListModel(requestType)
                visible: comboVisible(requestType)
            }
        }
        RowLayout{
            Layout.fillWidth: true;
            Text{
                text:qsTr("Scene name")
                color:"#fff";
            }
            TextField{
                id:nameEdit
            }
        }
        RowLayout{
            spacing: 10
            Button{
                text: qsTr("Add")
                onClicked: {
                    var msg = new Object();
                    msg.addScene = new Object();
                    msg.addScene.name = nameEdit.text;
                    msg.addScene.type = correctTypeChooser(requestType); //sceneTypeModel.get(sceneTypeComo.currentIndex).type;
                    function correctTypeChooser(requestType){
                        return chooseCorrectListModel(requestType).get(sceneTypeComo.currentIndex).type;
                    }

                    console.log("requestType: "+requestType)
                    console.log("chosen ListModel: "+chooseCorrectListModel(requestType))
                    console.log("sceneType: "+chooseCorrectListModel(requestType).get(sceneTypeComo.currentIndex).type)
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

