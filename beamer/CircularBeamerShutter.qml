import QtQuick 2.0
import WebSocketConnector 1.1

Item{
    anchors.fill: parent;
    property bool follow:true;
    Rectangle{
        id: pointer
        color:'#ffffff'
        opacity: 1
        width: parent.height;
        height: width;
        //anchors.centerIn: parent
        radius: width/2
       /* Behavior on x{
            NumberAnimation{
                duration: 100;
            }
        }
        Behavior on y{
            NumberAnimation{
                duration: 100;
            }
        }*/
        onXChanged: {
            if(follow)
                main.centerX = (x + width/2)/parent.width;
            else main.centerX = 0.5;
        }
        onYChanged: {
            if(follow)
                main.centerY = (y + height/2)/parent.height;
            else
                main.centerY = 0.5;
        }
    }
    WebSocketConnector{
        id: wsc3
        requestTypePassive: "asBeamer"
        onMessage: {
            if(msg.shutterType !== undefined && msg.shutterType === "circular"){
                if(msg.shutterParams.x !== undefined)
                    pointer.x =  (msg.shutterParams.x*(width-pointer.width));//-(pointer.width/2);
                if(msg.shutterParams.y !== undefined)
                    pointer.y =  (msg.shutterParams.y*(height-pointer.height));//-(pointer.height/2);
                if(msg.shutterParams.radius !== undefined)
                    pointer.width = msg.shutterParams.radius*height;
            }
        }
    }
}

