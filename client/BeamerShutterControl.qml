import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.2
import WebSocketConnector 1.0
Item{
ListModel{
    id:effectListModel
    ListElement{
        name:"manual Spot"
    }

    ListElement{
        name:"color bar"
    }
    ListElement{
        name:"color bar"
    }
    ListElement{
        name:"color bar"
    }
    ListElement{
        name:"color bar"
    }
    ListElement{
        name:"color bar"
    }
    ListElement{
        name:"color bar"
    }


}

Component{
    id:effectListDelegate
    Rectangle {
        gradient: Gradient{
             GradientStop { position: 0.0; color: "#000" }
             GradientStop { position: 1.0; color: "#333" }
        }
        border.width: 1
        border.color: "#444"
        width:effectListView.width
        height:text.height + 20
        Text{
            anchors.centerIn: parent
            id:text
            text:name;
            color:"#fff"
            font.pointSize: 15;
        }
    }
}

Rectangle{
    anchors.fill: parent;
    color:"#333";
    width: parent.width
    height: parent.height

    RowLayout{
        anchors.fill: parent;
        Rectangle{
            width:200;
            color:"#222"
            Layout.fillHeight: true;
            ListView{
                id: effectListView
                width: parent.width
                height:parent.height
                model: effectListModel
                delegate: effectListDelegate
            }
        }
        Rectangle{
            color:"#333"
            Layout.fillHeight: true;
            Layout.fillWidth: true;

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
                    text:"Touch Control"
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
                       x: point1.x - width/2
                       y: point1.y - height/2
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

                       onCenterXChanged: validateX()

                       function validateY(){
                           if(centerY < height/2)
                               centerY = height/2;
                           else if(centerY > tArea.height-height/2)
                              centerY = tArea.height-height/2;
                           y = centerY - height/2;
                       }

                       onCenterYChanged: validateY()

                       onHeightChanged: validateY()
                   }

                }
            Rectangle{
                Layout.fillHeight: true
                Layout.fillWidth: true
                color:"#333"
                RowLayout{
                    anchors.fill: parent
                    Text{
                        text:"Radius:"
                        font.pointSize: 13
                        color:"#fff"
                    }

                    Slider{
                        maximumValue:tArea.width
                        minimumValue: tArea.width/10
                        onValueChanged: {
                            console.log(value);
                            green.width = value;
                        }
                        width: 200
                    }
                }
            }
            }
        }

    }
/*
    RowLayout{
        anchors.fill: parent;
        Rectangle{
            anchors.fill: parent
            Layout.fillHeight: true;
            width:200;
            ListView{
                id: effectListView
                width: parent.width
                height:parent.height
                model: effectListModel
                delegate: effectListDelegate
            }

        }
        Rectangle{
            Layout.fillWidth: true
            Layout.fillHeight: true
            anchors.fill: parent
            Rectangle{
                width:100
                height:100
                //anchors.verticalCenter: parent;
                //anchors.top:parent;
                anchors.topMargin: 30;
                color:"#444"
            }
        }


    }*/
}

WebSocketConnector{
    url:"asdf"
}

}
