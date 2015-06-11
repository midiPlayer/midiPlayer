import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3

Item {
    id: item1
    width: 640
    height: 480
    property alias connectBtn: connectBtn
    property alias urlEdit: urlEdit



    Item {
        id: item2
        x:(parent.width-width)/5
        y:(parent.height-height)/4
        ColumnLayout {
            spacing:30
        Text {
            id: text1
            x: 91
            y: 66
            width: 468
            height: 86
            text: qsTr("Connect")
            style: Text.Normal
            font.pointSize: 56
            font.bold: true
            color:"#369cb6"
            opacity: 0.5
        }
        RowLayout{
        TextField {
            id: urlEdit
            Layout.minimumWidth: 150
            placeholderText: qsTr("z.B. ws:192.168.0.1:8080")
           /* style: TextFieldStyle {
                textColor: "#fff"
                background: Rectangle {
                            implicitWidth: 100
                            radius:2
                            implicitHeight: 24
                            border.color: "#777"
                            border.width: 1
                            color:"#11000000"
                            }
            }*/
        }
        Button {
            id: connectBtn
            text: qsTr("Connect")

        }
        }
        }

    }
    states: [
        State {
            name: "connecting"

            PropertyChanges {
                target: text1
                text: qsTr("Connect")
            }

            PropertyChanges {
                target: connectBtn
                text: qsTr("Connecting")
                enabled: false
            }

            PropertyChanges {
                target: urlEdit
                readOnly: true
            }
        },
        State {
            name: "failed"
            PropertyChanges {
                target: urlEdit
                textColor: "#a00"
            }
        },
        State {
            name: "connected"
            PropertyChanges {
                target: text1
                text: qsTr("Connect")
            }

            PropertyChanges {
                target: connectBtn
                text: qsTr("Disconnect")
                enabled: true
            }

            PropertyChanges {
                target: urlEdit
                readOnly: true
            }
        }
    ]






}
