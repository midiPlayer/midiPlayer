import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.WebSockets 1.0
import QtQuick.Layouts 1.1


Item {
            property WebSocket ws
            property string address
            width: parent.width
            height: parent.height
            MainForm {
                anchors.fill: parent
                id: mainForm;
                button1.onClicked: {ws.sendTextMessage("trigger")}
                button2.onClicked: stackView.push(Qt.resolvedUrl("VisualisationOverviewView.qml"),{ address: urlEdit.text })
                button3.onClicked: messageDialog.show(qsTr("Button 3 pressed"))
                connectBtn.onClicked: {
                    address = "ws://localhost:8888"
                    var obj = Qt.createComponent("myWebSocket.qml",0,this);
                    ws = obj.createObject(this,{address:"ws://localhost:8888"});

                    console.log(ws)
                }
            }

            MessageDialog {
                id: messageDialog
                title: qsTr("May I have your attention, please?")

                function show(caption) {
                    messageDialog.text = caption;
                    messageDialog.open();
                }
            }
        }
