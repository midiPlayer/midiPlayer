import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.2
import WebSocketConnector 1.1

Item{
    anchors.fill: parent;
    property alias requestId : circularWsc.requestId
    ColumnLayout{
        anchors.fill: parent;
    Rectangle{
        anchors.horizontalCenter: parent.horizontalCenter
        width:400
        height:400
        color:"#444"
        Text{
            anchors.centerIn: parent;
            color:"#555"
            text:qsTr("Touch Control")
            font.pointSize: 18;
        }

        MouseArea{
            id: tArea
            anchors.fill: parent
            focus:true
            onMouseXChanged: {
                green.centerX = mouseX;
                green.centerY = mouseY;
            }

        }


        RectangularGlow {
            id: effect
            anchors.fill: green
            glowRadius: 1
            spread: 0.001
            color: "white"
            cornerRadius: green.radius + glowRadius
        }
        Rectangle {
            id: green
               width: 20; height: width
               color: "white"
               radius: width/2
               opacity: 0.5
               x: parent.x - width/2
               y: parent.y - height/2
               property int centerX
               property int centerY


               function validateX(){
                   if(centerX < width/2)
                       centerX = width/2;
                   else if(centerX > tArea.width-width/2)
                      centerX  = tArea.width-width/2;
                   x = centerX-width/2;
               }

               onWidthChanged: validateX()

               onCenterXChanged: {
                   validateX()
               }

               property int lastSendTime: 0

               onXChanged: positionChanged();
               onYChanged: positionChanged();

               function positionChanged(){
                   var d = new Date();
                   var now = d.getTime();
                   if(now-lastSendTime < 5)
                       return;//too often
                   lastSendTime = now;

                  var msg = new Object();
                   msg.centerChanged = new Object();
                   msg.centerChanged.x = x/(parent.width-width);
                   msg.centerChanged.y = y/(parent.height-height);
                   circularWsc.send = JSON.stringify(msg);
               }

               function validateY(){
                   if(centerY < height/2)
                       centerY = height/2;
                   else if(centerY > tArea.height-height/2)
                      centerY = tArea.height-height/2;
                   y = centerY - height/2;
               }

               onCenterYChanged: {
                   validateY()
               }

               onHeightChanged: validateY()
           }

        }
    Item{
        Layout.fillHeight: true
        Layout.fillWidth: true
        RowLayout{
            anchors.fill: parent
            Text{
                text:qsTr("Radius")
                font.pointSize: 13
                color:"#fff"
            }

            Slider{
                id:radiusSlider
                maximumValue:1
                minimumValue: 0.1
                Layout.preferredWidth: 200
                onValueChanged: {
                    console.log(value);
                    green.width = value*tArea.width;
                    if(!pressed)
                        return;
                    var msg = new Object();
                    msg.radiusChanged = value;
                    if(circularWsc !== undefined)
                        circularWsc.send = JSON.stringify(msg);
                }
                width: 200
            }
        }
    }
    }
    WebSocketConnector{
        id:circularWsc
        onMessage: {
            console.log(msg);
            if(msg.radiusChanged !== undefined){
                radiusSlider.value = msg.radiusChanged;
            }
        }
    }
}
