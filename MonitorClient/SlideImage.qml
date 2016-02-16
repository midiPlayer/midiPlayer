import QtQuick 2.0

Item {
    id:container

    anchors.fill: parent;
    property alias source: image.source
    property int fadeDur : 500
    function fadeIn(done){
        animation.done = done;
        animation.running = true;
    }

    function fadeOut(){
        animationFadeOut.running = true;
    }


    Image{
        id:image
        anchors.fill: parent;
        fillMode: Image.PreserveAspectCrop

        PropertyAnimation {
            id: animationFadeOut;
            target: container;
            property: "opacity";
            to: 0;
            duration: fadeDur
        }

        PropertyAnimation {
            id: animation;
            target: container;
            property: "opacity";
            to: 1;
            duration: fadeDur

            property var done;

            onRunningChanged: {
                if(running == false){
                    done();
                    done = function(){};
                }
            }
        }
    }
}
