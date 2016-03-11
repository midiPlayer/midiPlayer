import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import RGBWColor 1.1


Item {
    property string color;

    RGBWColor{

    }

    ColumnLayout{
        //width:600;
        spacing: 20
        anchors.centerIn: parent;
        Text{
            font.pixelSize: 30
            text:qsTr("Custommize Color")
            color: "#369cb6"
        }

        GridLayout{
            Layout.fillWidth: true;
            columns: 2
            Text{
                text:qsTr("Red")
                color: "#369cb6"
            }
            Slider{
                id:redSlider
                minimumValue: 0
                maximumValue: 255
            }
            Button{
                text:qsTr("Done")
            }


        }
    }
}
