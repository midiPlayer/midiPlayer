import QtQuick 2.0

import QtQuick 2.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1

Item{
    width: parent.width;
    height:200
    id:main
    property alias requestId : wsc.requestId

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20

            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Trigger:")
                    color:"#fff";
                }
                TriggerSourceBtn{
                    id:triggerBtn
                }
            }

    }


    WebSocketConnector{
        id: wsc
        onMessage: {
            if(msg.config.trigger !== undefined){
                triggerBtn.requestId = msg.config.trigger;
            }

        }
    }

}
