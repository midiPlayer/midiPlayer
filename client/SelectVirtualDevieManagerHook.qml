import QtQuick 2.0
import QtQuick.Controls 1.4

    Button{
        property int requestId;
        text: qsTr("select Devices")
        onClicked: {
            stackView.push(Qt.resolvedUrl("SelectVirtualDevieManager.qml"),{"requestId":requestId});
        }
    }
