import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: item1
    width: 640
    height: 480

    property alias button3: button3
    property alias button2: button2
    property alias button1: button1
    property alias connectBtn: connectBtn
    property alias urlEdit: urlEdit

    RowLayout {
        id: rowLayout1
        width: 444
        height: 22
        transformOrigin: Item.Center
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 237
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 8

        Button {
            id: button1
            text: qsTr("Trigger")
            enabled: false
        }

        Button {
            id: button2
            text: qsTr("Press Me 2")
        }

        Button {
            id: button3
            text: qsTr("Press Me 3")
            anchors.right: parent.right
            anchors.rightMargin: 0
        }
    }

    Row {
        x: 195
        y: 201

        TextField {
            id: urlEdit
            width: 168
            height: 22
            visible: true
            placeholderText: qsTr("z.B. ws:192.168.0.1:8080")
        }

        Button {
            id: connectBtn
            text: qsTr("Verbinden")
        }
    }




}
