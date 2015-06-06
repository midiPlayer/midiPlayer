import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
Item{
    id: button
    property alias imageSrc : img.source
    property alias touchedImageSrc : imgTouched.source
    property bool rotate : false
    signal clicked();
    function triggered(){
        rotator.running = rotate;
    }

    Layout.alignment:Qt.AlignCenter
    Layout.preferredHeight: width
    //Layout.minimumWidth: 200

            Item{
                id: rotatable
                anchors.fill: parent
             Image{
                 id: img
                 anchors.fill: parent
                 fillMode: Image.PreserveAspectFit
             }
             Image{
                 id: imgTouched
                 anchors.fill: parent
                 fillMode: Image.PreserveAspectFit
                 opacity: 0
                 Behavior on opacity {
                     NumberAnimation{
                         duration: 50
                     }
                 }
             }
             RotationAnimator {
                     id:rotator
                     target: rotatable
                     from: 0;
                     easing.type: Easing.InOutCubic
                     to: 360;
                     duration: 1000
              }

             }


    MouseArea{
        anchors.fill: parent
        onClicked: button.clicked();
        onPressed: {
            imgTouched.opacity = 1;
        }
        onReleased: {
            imgTouched.opacity = 0;
            rotator.running = rotate;
        }
        onExited: {
            imgTouched.opacity = 0;
        }
    }
}
