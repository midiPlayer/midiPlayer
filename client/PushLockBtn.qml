import QtQuick 2.0

Item{
        id: pushLockBtn
        width: 70; height: 0.5*width
        signal stateOn()
        signal stateOff()
        property bool mute : false
        property bool isOn : false;
        onIsOnChanged: {
            if(isOn)
                setOn(false);
            else
                setOff(false);
        }

        function setOn(trigger){
            slider.setOn(trigger);
        }
        function setOff(trigger){
            slider.setOff(trigger);
        }

        Component.onCompleted: {
            if(mute){
            slider.touchedC1 = "#b70b48";
            slider.touchedC2 = "#97093b";

            slider.stdC1 = "#888";
            slider.stdC2 = "#666";
            textF.text = "A" //active
            dot.color = "#b70b48";
            }

        }

    Rectangle {
        width: parent.width; height: parent.height
        color: "grey"
        radius: 8



         Rectangle {/** the small dot when active **/
             id: dot
              width: parent.width * 0.1
              height: parent.width * 0.1
              x:parent.width*0.15
              y:parent.height/2-height/2
              color: "#2f757f"
              radius: width*0.5
         }

         gradient: Gradient {
                  GradientStop { position: 0.0; color: "#aaa"}
                  GradientStop { position: 1.0; color: "#999"}
         }

        Rectangle {
            id: slider
            x: 0; y: 0
            width: 0.6*parent.width; height: 0.5*parent.width
            color: "red"

            property color stdC1: "#26a5b8";
            property color stdC2: "#5497a1";

            property color touchedC1: "#2f757f";
            property color touchedC2: "#426d74";

            property Gradient gradientTouched: Gradient {
                GradientStop { position: 0.0; color: slider.touchedC1 }
                GradientStop { position: 1.0; color: slider.touchedC2 }
            }

            property Gradient gradientStd: Gradient {
                GradientStop { position: 0.0; color: slider.stdC1 }
                GradientStop { position: 1.0; color: slider.stdC2 }
            }

            gradient: gradientStd

            function setOff(trigger){
                state = "off";
                x = 0;
                if(trigger)
                    pushLockBtn.stateOff();
            }

            function setOn(trigger){
                state = "on";
                x = (parent.width - width);
                if(trigger)
                    pushLockBtn.stateOn();
            }

            Text{
                id: textF
               text: "S"
               color:"#fff"
               font.pixelSize: parent.height*0.7
               verticalAlignment: Text.AlignVCenter
               horizontalAlignment: Text.AlignHCenter
               width:parent.width
               height:parent.height
            }

            radius: 8

            Behavior on x {
                    id: bh1
                    NumberAnimation { duration: 100 }
                }
            states: [State {
                    name: "on"; PropertyChanges { target: slider; gradient: gradientTouched }
                },
                State {
                    name: "off"; PropertyChanges { target: slider;  }
                }
            ]

            MouseArea {
                id: dragArea
                anchors.fill: parent
                drag.axis: Drag.XAxis
                drag.target: parent
                drag.maximumX: parent.parent.width - parent.width;
                drag.minimumX: 0
                onPressed: {
                    if(mute){
                        if(parent.x > (parent.parent.width - parent.width) / 2){
                            pushLockBtn.stateOff();
                            parent.state = "off";
                        }
                        else{
                            pushLockBtn.stateOn();
                            parent.state = "on";
                        }
                    }
                    else{
                        if(parent.state != "on")
                            pushLockBtn.stateOn();
                            parent.state = "on";
                    }
                }
                onReleased: {
                    if(parent.x > (parent.parent.width - parent.width) / 2){
                        slider.setOn(true);
                    }
                    else{
                        slider.setOff(true)
                    }
                }
            }
        }

    }

    }


