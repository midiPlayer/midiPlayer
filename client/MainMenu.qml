import QtQuick 2.4
import QtQuick.Layouts 1.1

Item {
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
            link:"ConnectView.qml"
        }

        MainMenuItem{
            imageSrc: "icons/settings.png"
            name: "beamer"
            link:"BeamerShutterControl.qml"
        }
    }
   }
}

