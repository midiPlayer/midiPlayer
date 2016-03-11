import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import RGBWColor 1.1


Item {
    //property string color;
    property alias color : rgbwColor.string

    RGBWColor{
        id:rgbwColor
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
                minimumValue: 0
                maximumValue: 1
                value:rgbwColor.r;
                onValueChanged: {
                    rgbwColor.r = value;
                }
            }
            Text{
                text:qsTr("Green")
                color: "#369cb6"
            }
            Slider{
                minimumValue: 0
                maximumValue: 1
                value:rgbwColor.g;
                onValueChanged: {
                    rgbwColor.g = value;
                }
            }
            Text{
                text:qsTr("Blue")
                color: "#369cb6"
            }
            Slider{
                minimumValue: 0
                maximumValue: 1
                value:rgbwColor.b;
                onValueChanged: {
                    rgbwColor.b = value;
                }
            }
        }
            Rectangle{
                id:previwRect;
                height: 20
                Layout.fillWidth: true;
                color:rgbwColor.preview
            }

            Button{
                text:qsTr("Done")
                onClicked: {
                    console.log(rgbwColor.preview);
                    colorPicker.colorSelected(rgbwColor.preview)
                    colorPickerStackView.pop(null);
                }
            }


    }
}
