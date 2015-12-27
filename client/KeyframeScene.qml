import QtQuick 2.0
import WebSocketConnector 1.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {
    property alias requestId : ws.requestId
    width: parent.width
    height: parent.height

    ListModel {
        id:deviceModell
    }

    Item{
        id:deviceView
        width: parent.width
        height: parent.height/2
        ScrollView{
            anchors.fill: parent
            ListView {
                width: 180; height: 200

                model: deviceModell
                delegate: Item{
                    width: parent.width
                    height: 60
                    RowLayout{
                        anchors.fill: parent

                        Text {
                            text: devID
                            Layout.fillWidth: true
                            color:"#369cb6"
                        }
                        CheckBox{
                            onCheckedChanged: {
                                console.log(devID + "changed");
                            }
                        }
                    }
                }
            }

        }
    }

    WorkerScript {
            id: deviceWorker
            source: "KeyframeSceneDeviceWorker.js"
   }

    WebSocketConnector{
        id: ws
        onMessage: {
            console.log(JSON.stringify(msg));
            if(msg.devices !== null)
                deviceWorker.sendMessage({"model":deviceModell,"lamps":msg.devices});
            //importer.sendMessage({"msg":msg,"listModel":listModel,"fusionTypeModel":fusionTypeModel});
        }
    }

    Item{//graph display
        width: parent.width;
        height: parent.height/2;
        anchors.top:deviceView.bottom

        GraphViewer{
            id:viewer
        }
    }
}
