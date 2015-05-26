import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item{
    property ColorPickerButton button
    WorkerScript {
            id: addColor
            source: "ColorPickerAddColor.js"
        }

    function save(){
        var colorString=""
        for(var i = 0; i < selectedColorLVModel.count; i++){
            if(i!=0) colorString+=","
            colorString+=selectedColorLVModel.get(i).color
        }

        button.myColor=colorString
    }

    Component.onCompleted: {
        applicationWindow.backPressHandler=function() {
        if (colorPickerStackView.depth > 1) //d.h. im Submenu
        {
            colorPickerStackView.pop();
            return true;
        }
        else {
            applicationWindow.backPressHandler=function() {
                return false;
            }
            save()

            return false;
        }
        }
        if(button.myColor!=""){
            var colorsOfButton=button.myColor.split(",");
            for(var i = 0; i < colorsOfButton.length; i++){
            addColor.sendMessage({"action":"add","color":colorsOfButton[i],"model":selectedColorLVModel})
            }
        }
    }

    ColumnLayout{
        id: colorPicker
        function colorSelected(color) {
            if(button.onlyOne){
                button.myColor = color;
                stackView.pop()
            }
            else{
                console.log(selectedColorLVModel.count)
                var found = false;
                for(var i = 0; i < selectedColorLVModel.count; i++){
                   if(color==selectedColorLVModel.get(i).color){
                       found=true
                       console.log("keine doppelten Farben!")
                       break
                   }
                }
                if(found == false) {
                   addColor.sendMessage({"action":"add","color":color,"model":selectedColorLVModel})
                }
            }




        }

        anchors.fill: parent
        Item{
            Layout.preferredHeight: 100
            Layout.fillWidth: true


            ListView{
                id: selectedColorLV
                model: ListModel{
                    id: selectedColorLVModel
                   /* ListElement{
                        color: "red"
                    }
                    */
                }
                anchors.fill: parent
                orientation: Qt.Horizontal
                layoutDirection: Qt.LeftToRight

                delegate:

                 Item{

                    property int myIndex: index
                    //Layout.maximumWidth: 300
                    //Layout.minimumWidth: 100
                    //height : parent.height;
                    width: 100
                    height: parent.height
                        ColorButton{
                            anchors.centerIn: parent
                            width: parent.width*0.9
                            height: parent.height*0.9
                            bcolor: color
                            onSelected: {
                                addColor.sendMessage({"action":"delete", "model":selectedColorLVModel, "myIndex":index})
                            }
                        }
                }

            }
        }

            StackView{
                Layout.fillHeight: true
                Layout.fillWidth: true
                id: colorPickerStackView
                Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true
                }
              initialItem: initial
              }
    }

    property ColorPickerView initial: ColorPickerView{
        lmodel: ListModel{ 
            ListElement{
                color: "blue"
                subcolors: '["#2020c4","#1883de","#2c58d5"]'
            }

            ListElement{
                color: "#03D1D1"
            }

            ListElement{
                color: "#16E635"
            }

            ListElement{
                color: "green"
            }

            ListElement{
                color: "yellow"
            }

            ListElement{
                color: "orange"
            }

            ListElement{
                color: "red"
            }

            ListElement{
                color: "purple"
            }

            ListElement{
                color: "grey"
            }

        }
    }
}
