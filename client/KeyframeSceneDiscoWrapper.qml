import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import WebSocketConnector 1.1

Item{
    width: parent.width;
    height:200
    id:main
    property int requestId;

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20

            RowLayout{
                Button{
                    text: qsTr("Edit")
                    onClicked: {
                        stackView.push(Qt.resolvedUrl("KeyframeScene.qml"),{"requestId":requestId});
                    }
                }
            }
    }
}

