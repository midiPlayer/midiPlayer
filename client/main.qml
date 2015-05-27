import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.WebSockets 1.0
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: applicationWindow
    title: qsTr("light client")
    width: 640
    height: 600
    visible: true
    property var backPressHandler: function() {return false};

    color:"#002b38"



    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }


ColumnLayout{
    anchors.fill: parent;
    Item{
        id:backBtn
        Layout.preferredWidth: 40
        Layout.preferredHeight: 40
        Behavior on opacity{
            NumberAnimation{
                duration: 200
            }
        }

        Image {
            source: "icons/back.png"
           fillMode: Image.PreserveAspectFit
            height: parent.height * 0.6
            anchors.centerIn: parent
        }
        Image {
            id: touched
            source: "icons/backTouched.png"
           fillMode: Image.PreserveAspectFit
            height: parent.height * 0.6
            anchors.centerIn: parent
            visible: false
        }
        MouseArea{
            anchors.fill: parent;
            onPressed: {
                touched.visible = true;
                if(!backPressHandler())
                    stackView.pop()

            }
            onReleased: {
                touched.visible = false;
            }
            onCanceled: {
                touched.visible = false;
            }
            onExited: {
                touched.visible = false;
            }
        }
    }

    StackView {
        id: stackView
        Layout.fillWidth: true;
        Layout.fillHeight: true;
        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             if(!backPressHandler())
                                 stackView.pop()
                             event.accepted = true;
                         }

        initialItem: Qt.resolvedUrl("ConnectView.qml")
        onDepthChanged: {
            backBtn.opacity = (depth > 1)*1;
        }
    }
    }
}


