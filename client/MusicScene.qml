import QtQuick 2.0

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import WebSocketConnector 1.1

Item{
    width: parent.width;
    height:100
    id:main
    property alias requestId : wsc.requestId

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20

            RowLayout{
                width: parent.width
                Text{
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text:qsTr("Color")
                    color:"#fff";
                }
                MusicPlayer{
                    id:player
                }
            }

    }


    WebSocketConnector{
        id: wsc
        onMessage: {
            if(msg.hasOwnProperty("player")){
                player.requestId = msg.player;
            }
        }
    }

}

