import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.2
import WebSocketConnector 1.1
Item{
ListModel{
    id:effectListModel
    /*ListElement{
        name:"manual Spot"
    }*/
}

Component{
    id:effectListDelegate
    Rectangle {
        color:"#000000ee"
        border.width: 1
        border.color: "#111"
        width:effectListView.width
        height:text.height + 20
        MouseArea{
            anchors.fill: parent;
            onClicked: {
                var filename = "";
                if(modelData.requestType === "circualrBeamerShutter")
                    filename = "CircularShutterScene.qml";
                if(filename != ""){
                    var obj = Qt.createComponent(filename,0,placeholder);
                    obj.incubateObject(placeholder,{"visible":true,"requestId":modelData.requestId});
                }
            }
        }

        RowLayout{
            anchors.fill: parent;
            anchors.leftMargin: 10
            Item{
                Layout.fillHeight: true;
                Layout.preferredWidth: 20;
                Rectangle{
                    anchors.centerIn: parent;
                    width: 15;
                    height: width;
                    radius: width/2;
                    color:"#369cb6"
                    visible: modelData.isAcitve
                }
            }

            Text{
                anchors.centerIn: parent
                id:text
                text:modelData.name;
                color:"#369cb6"
                font.pointSize: 15;
            }
        }
    }
}

Item{
    anchors.fill: parent;
    width: parent.width
    height: parent.height

    RowLayout{
        anchors.fill: parent;
        Item{
            width:200;
            Layout.fillHeight: true;
            ListView{
                id: effectListView
                width: parent.width
                height:parent.height
                model: effectListModel
                delegate: effectListDelegate
            }
        }

    Item{
        Layout.fillHeight: true;
        Layout.fillWidth: true;
        id:placeholder
    }
    }

}
WorkerScript {
        id: importer
        source: "BeamerShutterControlImporter.js"
        signal fusionTypeChanged();
    }

WebSocketConnector{
    requestType:"BeamerShutterControl"
    onMessage: {
        if(msg.effectList !== undefined){
            importer.sendMessage({"msg":msg,"listModel":effectListModel});
        }
    }
}

}
