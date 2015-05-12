import QtQuick 2.4
import QtQuick.Layouts 1.1

Rectangle {
    color:"#002b38"
    RowLayout{
        anchors.fill: parent
    GridLayout {
        columns: 3
        Layout.alignment:Qt.AlignCenter
        Layout.minimumWidth: 300
        Layout.preferredWidth: 800
        Layout.preferredHeight: 500

        MainMenuItem{
            imageSrc: "icons/settings.png"
            name: "current Scene"
            link:"DiscoScene.qml"
        }

        MainMenuItem{
            imageSrc: "icons/trigger.png"
            name: "configutre Trigger"
            link:""
        }

        MainMenuItem{
            imageSrc: "icons/settings.png"
            name: "Beamer"
            link:"BeamerShutterControl.qml"
        }
    }
   }
}

