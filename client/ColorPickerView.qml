import QtQuick 2.0
import QtQuick.Layouts 1.1

    Item{
        property int qw:3
        property int qh:3
        id:size
        //anchors.fill: parent
        property alias lmodel:gv.model
        onWidthChanged: {
            gv.cellWidth= width/qw
         }
        onHeightChanged: {
            gv.cellHeight=height/qh
        }
        signal colorSelected(string color);


    GridView{

        id:gv
        anchors.fill: parent
        Component.onCompleted: {

            parent.qw = Math.ceil(Math.sqrt(model.count));
            parent.qh = Math.ceil(model.count/parent.qw);
            cellWidth= width/parent.qw
            cellHeight=height/parent.qh

        }

        delegate:
         Item{
            width : gv.cellWidth;
            height : gv.cellHeight;
            property int myIndex: index
                ColorButton{
                    anchors.centerIn: parent
                    width: parent.width*0.9
                    height: parent.height*0.9
                    bcolor: color
                    subColorsJson: {
                        if(subcolors != undefined)  return subcolors;
                        return "";
                    }
                    onSelected: {
                        if(subColors.count == 0)
                        {
                            colorPicker.colorSelected(color)
                            colorPickerStackView.pop(null);
                        }
                        else{
                            colorPickerStackView.push(Qt.resolvedUrl("ColorPickerView.qml"),{"lmodel":subColors});

                        }
                    }
                }
        }
    }
}
