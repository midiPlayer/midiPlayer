import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Qt.WebSockets 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true
   // visibility: "FullScreen"

    Item{
        anchors.fill: parent
        focus:true;
        Keys.onPressed: {
            main.color = "green";
        }

    Rectangle{
        id:mask
        color:"#00000000"
        anchors.fill: parent
        Image {
                id:linien
                source: "blenden/linien.png"
                anchors.centerIn: parent
                width:Math.sqrt(parent.width*parent.width+parent.height*parent.height);
                height:Math.sqrt(parent.width*parent.width+parent.height*parent.height);
                sourceSize: Qt.size(parent.width, parent.height)
                smooth: true
                fillMode: Image.PreserveAspectCrop
                RotationAnimator {
                        target: linien
                        from: 0;
                        to: 360;
                        duration: 20000
                        running: true
                        loops: Animation.Infinite;

                    }

            }


        visible: false
    }



    Rectangle{
        id:mainBG
        width:100
        height:100
        anchors.fill: parent
        color:'red'
        visible: false
    }


    Rectangle{
        id:blackOvrlay
        anchors.fill: parent
        color:'black'
        visible: false;

    }

    Rectangle{
        id:blackMask
        anchors.fill: parent
        color:"#00000000"
        visible: false;
        Rectangle{
            id: pointer
            color:'#ffffff'
            opacity: 1
            width: parent.height;
            height: parent.height;
            anchors.centerIn: parent
            radius: width/2
        }
    }


    Rectangle{
    id: main
    anchors.fill: parent
    color:"blue"
    OpacityMask{
        anchors.fill: parent
        source:mainBG
        maskSource:mask
    }
    visible: false
    }

    Rectangle{
        anchors.fill: parent
        color:"black"
    }


    OpacityMask{
        id: display
        anchors.fill: parent
        source:main
        maskSource:blackMask
        visible: true
    }

}

    WebSocket {
            id: socket
            url: "ws://localhost:8888"
            onTextMessageReceived: {
                //messageBox.text = messageBox.text + "\nReceived message: " + message
                //main.color = message
                var msg= JSON.parse(message);
                console.log(msg);
                if(msg.data.color !== undefined){
                    main.color = msg.data.color
                }


            }
            onStatusChanged: if (socket.status == WebSocket.Error) {
                                display.visible = false;
                             } else if (socket.status == WebSocket.Open) {
                                 var message = new Object();
                                 message.register = ["asBeamer"];
                                 message.parameters = new Object();
                                 message.parameters.deviceId = "beamer1";
                                 var json = JSON.stringify(message);
                                 console.log("connected");
                                 socket.sendTextMessage(json);
                             } else if (socket.status == WebSocket.Closed) {
                                 display.visible = false;
                             }
            active: true
        }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

}
