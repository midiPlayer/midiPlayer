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

    SplitView{
    anchors.fill: parent
    orientation: Qt.Vertical
    Item{
        id:deviceView
        height: parent.height/2
        Layout.minimumHeight:  150
     //   width: parent.width
     //   height: parent.height/2
        ScrollView{
            anchors.fill: parent
            ListView {
                width: 180; height: 200

                id:lv


                property string copyedDev;
                property string buttonState : "copy";


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

                        Button{
                            text: qsTr("clear")
                            onClicked: {
                                var msg = new Object();
                                msg.clear = devID;
                                ws.send = JSON.stringify(msg);
                            }
                        }

                        Button{
                            id:copyBtn
                            state:lv.buttonState

                            states: [
                                State{
                                    name: "copy"
                                    PropertyChanges{
                                        target: copyBtn;
                                        text: qsTr("copy");
                                        onClicked:{
                                            lv.buttonState = "paste";
                                            copyBtn.state = "copyed"
                                            lv.copyedDev = devID;
                                        }
                                        state:lv.buttonState
                                    }
                                },
                                State{
                                    name: "paste"
                                    PropertyChanges{
                                        target: copyBtn;
                                        text: qsTr("paste");
                                        onClicked:{
                                            console.log("copy " + lv.copyedDev + " to " + devID);
                                            var msg = new Object();
                                            msg.copy = new Object();
                                            msg.copy.from = lv.copyedDev;
                                            msg.copy.to = devID;
                                            ws.send = JSON.stringify(msg);
                                        }
                                        state:lv.buttonState;
                                    }
                                },
                                State{
                                    name:"copyed"
                                    PropertyChanges{
                                        target: copyBtn;
                                        text: qsTr("end copy");
                                        onClicked:{
                                            state = "copy";
                                            lv.buttonState = "copy";
                                        }
                                    }
                                }

                            ]
                        }

                        CheckBox{
                            onCheckedChanged: {
                                console.log(devID + "changed");
                                if(checked)
                                    graphViewer.addDevice(devID);
                                else
                                    graphViewer.rmDevice(devID);
                            }
                        }
                    }
                }
            }

        }
    }


    Item{//graph display
        //width: parent.width;
        //height: parent.height/2;
        //anchors.top:deviceView.bottom

        Layout.minimumHeight: parent.height / 4
        GraphViewer{
            id:graphViewer
            requestId: ws.requestId
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
            if(msg.hasOwnProperty("devices"))
                deviceWorker.sendMessage({"model":deviceModell,"lamps":msg.devices});
        }
    }
}
