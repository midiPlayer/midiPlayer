import QtQuick 2.0
import QtQuick.Dialogs 1.2

Rectangle {
    id: button
    property bool checked: false
    Accessible.onPressAction: {
        button.clicked()
    }

    signal clicked
    width: buttonText.width + 20
    height: 30
    gradient: Gradient {
        GradientStop { position: 0.0; color: "white" }
        GradientStop { position: 1.0;
            color: button.focus ? "white" : "blue" }
    }

    radius: 5
    antialiasing: true

    /**
    Text {
        id: buttonText
        text: "Hello"
        anchors.centerIn: parent
        font.pixelSize: parent.height * .5
        style: Text.Sunken
        color: "white"
        styleColor: "black"
    }
    */

    MessageDialog {
        id: messageDialog
        title: qsTr("Nachricht")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: messageDialog.show(qsTr("Hier kommen noch mehr Farben"));
    }

    Keys.onSpacePressed: clicked()
}
