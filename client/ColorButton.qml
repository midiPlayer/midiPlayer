import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property alias bcolor: r1.color
    signal selected();
    property string subColorsJson
    property ListModel subColors : ListModel{}

    Rectangle{
        id: r1
        anchors.fill: parent

        Image{
              id: shadow
              source: "images/schatten.png"
              anchors.fill: parent
              opacity: 0.6
        }
        visible: false

    }

    onSubColorsJsonChanged: {
        var arr = JSON.parse(subColorsJson);
        for(var i = 0; i < arr.length; i++){
            var colorObj = new Object();
            colorObj.color = arr[i];
            subColors.append(colorObj);
        }
    }

    Rectangle{
        id: mask
        //color: "green"
        radius: 11
        anchors.fill: parent
        visible: false

    }

    MouseArea{
        anchors.fill: parent
        onPressed: {
            shadow.opacity = 0.8;

        }
        onReleased: {
            shadow.opacity = 0.6;
            selected();
        }
        onCanceled: {
            shadow.opacity = 0.6;
        }
        onExited: {
            shadow.opacity = 0.6;
        }
    }

    OpacityMask{
        maskSource: mask
        source: r1
        anchors.fill: parent

    }

}

