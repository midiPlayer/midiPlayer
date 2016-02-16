import QtQuick 2.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1
import QtQuick.Controls 1.2

Component{
    id:diaListDelegate

    Rectangle {
        color:"#000000ee"
        width:parent.parent.width;
        height:text.height + 20
        MouseArea{
            anchors.fill: parent;
            property var pressedTstmp : -1;
            onPressed: {
                pressedTstmp = new Date().getTime();
            }

            onReleased: {
                if(pressedTstmp == -1)
                    return;
                var dur = new Date().getTime() - pressedTstmp;
                if(dur > 500){ //long click
                    diaList.currentIndex = index;
                    var msg = {"currentScene":id};
                    wsc.send = JSON.stringify(msg);
                    diaList.currentIndex = index;
                }
                else{//short click
                    diaWSC.requestId = requestId;
                    diaLayout.visible=true;
                    diaLayout.id = id;
                    diaLayout.index = index;
                }
            }
        }

        RowLayout{
            anchors.fill: parent;


            Text{
                anchors.centerIn: parent
                width: parent.parent.width;
                Layout.maximumWidth: parent.width;
                id:text
                text:name;
                color:"#369cb6"
                font.pointSize: 15;
                wrapMode: Text.WrapAnywhere;
            }
        }
    }
}
