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
                    text:qsTr("Foreground trigger:")
                    color:"#fff";
                }
                TriggerSourceBtn{
                    id:foregroundBtn
                }
            }
            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Background trigger:")
                    color:"#fff";
                }
                TriggerSourceBtn{
                    id:backroundBtn
                }
                }

    }


    WebSocketConnector{
        id: wsc
        onMessage: {
            if(msg.config.foregroundTrigger !== undefined){
                foregroundBtn.requestId = msg.config.foregroundTrigger;
            }
            if(msg.config.backgroundTrigger !== undefined)
                backroundBtn.requestId = msg.config.backgroundTrigger;

        }
    }

}
