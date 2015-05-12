import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.WebSockets 1.0
import QtQuick.Layouts 1.1
ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 600
    visible: true

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
        Layout.fillWidth: true;
        Layout.preferredHeight: 30
        Button{
            text: "back"
                onClicked: {
                    console.log(stackView.currentItem);
                    console.log(stackView.pop(stackView.currentItem.Stack.index));
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
                             stackView.pop();
                             event.accepted = true;
                         }

        initialItem: Qt.resolvedUrl("ConnectView.qml")
    }
    }
}


