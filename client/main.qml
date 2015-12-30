import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

ApplicationWindow {
    id: applicationWindow
    title: qsTr("LightClient")
    width: 640
    height: 600
    visible: true
    property var backPressHandler: function() {return false};

    color:"#002b38"

    IOManager {
        id: fileIO
    }

ColumnLayout{
    anchors.fill: parent;
    Keys.onPressed: {
        event.accepted = true;

        if(event.key === Qt.Key_S && event.modifiers === Qt.ControlModifier){
            console.log("STRG+S");
            fileIO.save();

        }
    }

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

            }
            onReleased: {
                touched.visible = false;
                if(!backPressHandler())
                    stackView.pop()

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
