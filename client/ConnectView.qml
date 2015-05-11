import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.WebSockets 1.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1
import Qt.labs.settings 1.0

Item {
            property WebSocket ws
            property string address
            width: parent.width
            height: parent.height
            ConnectViewForm {
                anchors.fill: parent
                id: mainForm;
                /*button1.onClicked: {ws.sendTextMessage("trigger")}
                button2.onClicked: stackView.push(Qt.resolvedUrl("DiscoScene.qml"),{ address: urlEdit.text })
                button3.onClicked: stackView.push(Qt.resolvedUrl("BeamerShutterControl.qml"),{  })*/
                connectBtn.onClicked: {
                  wsc.url = urlEdit.text;
                  mainForm.state = "connecting";
                }
                Component.onCompleted: {
                    urlEdit.text = settings.lastUrl;
                }
            }


            WebSocketConnector{
                id: wsc
                onConnectionSucceded: {
                    settings.lastUrl = url;
                    //change view
                    stackView.push(Qt.resolvedUrl("MainMenu.qml"),{  });

                }
                onConnectionFailed: {
                    mainForm.state = "failed";
                }
            }
            Settings {
                    id: settings
                    property string lastUrl: ""
                }
        }
