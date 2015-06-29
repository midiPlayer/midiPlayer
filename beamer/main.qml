import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import WebSocketConnector 1.1


ApplicationWindow {
    title: qsTr("Beamer")
    width: 640
    height: 480
    visible: true

   //  visibility: "FullScreen"



    Item{
        anchors.fill: parent
        focus:true;
        Keys.onPressed: {
            main.color = "green";
        }

    Item{
        id:mask
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
                        duration: 10000
                        running: true
                        loops: Animation.Infinite;

                    }

            }


        visible: false
    }



    Rectangle{
        id:mainHighLighted
        width:100
        height:100
        anchors.fill: parent
        color:'black'
        visible: false
    }


    Rectangle{
        id:blackOvrlay
        anchors.fill: parent
        color:"black"
        visible: false;

    }

    Item{
        id:blackMask
        anchors.fill: parent
        visible: false;
        CircularBeamerShutter{

        }
    }


    Rectangle{
    id: main
    anchors.fill: parent
    color:"blue"

    property double centerX;
    property double centerY;

        OpacityMask{
            source:mainHighLighted
            maskSource:mask
            x:parent.centerX*width - width/2;
            y:parent.centerY*height-height/2;
            width: parent.width
            height: parent.height
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
            visible: false
        }


}
    WebSocketConnector{
        id:wsc
        registrationParams: {
            var msg = { "deviceId" : "beamer1"};
            return JSON.stringify(msg);
        }
        reopen: true
        onConnectionSucceded: {
            requestType="asBeamer";
            display.visible = true;
        }
        onConnectionFailed: {
            display.visible = false;
        }

        url:"ws://127.0.0.1:8888"
        onMessage: {
            if(msg.color !== undefined){
                main.color = msg.color

            }
            if(msg.highlightedColor !== undefined){
                mainHighLighted.color = msg.highlightedColor

            }
        }
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
