import QtQuick 2.0

Rectangle{
    property color stdGradient: "#555"
    property color pressedGradient : "#999"
    property color triggeredGradient : "#30b3b8"
    property bool isFirst : false;
    property bool isLast : false;
    property string text;
    property bool isOn;
    property color textColor : "#fff";
    property color mColor : setGradient();

    property int cornerRadius: 0;

    signal stateOn();
    signal stateOff();


    onIsOnChanged: {
        mColor = setGradient();
    }


    function trigger(){
        anim1.start();
        anim2.start();
    }

    Component.onCompleted: {
        console.log("buttonItem completed");
    }

    function setGradient(){
        if(isOn)
           return pressedGradient;
        else
            return stdGradient;
    }


    color:"#00000000"

    height: parent.height


    Rectangle{
        id: rect1
        height: parent.height
        width: 2*cornerRadius * (isFirst + isLast)
        Component.onCompleted: {
            if(isLast)
                anchors.right= parent.right;
            else
                anchors.left= parent.left;
        }
        radius: cornerRadius
        color:parent.mColor
        ColorAnimation on color{
            running: false
            id: anim1
            duration :100
            from:triggeredGradient
            to: setGradient()
        }

    }
    Rectangle{
        id: rect2
        height: parent.height
        width: (parent.width - cornerRadius)*(isFirst+isLast) + parent.width * !(isFirst+isLast)
        Component.onCompleted: {
            if(isLast)
                anchors.left= parent.left;
            else
                anchors.right= parent.right;

        }
        radius: 0
        color:parent.mColor
            ColorAnimation on color{
                id: anim2
                running: false
                duration :100
                from:triggeredGradient
                to: setGradient()
            }

    }
    MouseArea{
        anchors.fill: parent;
        onPressed: {
            if(isOn){
                isOn = false;
                stateOff();
            }else{
                isOn = true;
                stateOn();
            }

            mColor = setGradient();
        }
        onReleased: {
        }
    }


    Text{
        text: parent.text;
        font.pointSize: 12
        color:textColor
        anchors.centerIn: parent
    }

}
