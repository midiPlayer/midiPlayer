import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

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

    MessageDialog{
        id: msgdialog
        title: qsTr("Message")
        text: qsTr("This Scene needs at least %L1 colors! Please choose some more!").arg(button.minColorNum)
        onAccepted: {
            visible = false;
        }
        Component.onCompleted: visible = false
   }

    Component.onCompleted: {
        applicationWindow.backPressHandler=function() {

        if (colorPickerStackView.depth > 1) //d.h. im Submenu
        {
            colorPickerStackView.pop();
            return true;
        }
        else {
            if(button.minColorNum > selectedColorLVModel.count){
                msgdialog.visible = true
                return true;
            }

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
        spacing: 10
        function colorSelected(color) {
            if(button.onlyOne){             //es darf (für diese Szene) nur eine Farbe ausgewählt werden
                button.myColor = color;
                applicationWindow.backPressHandler=function() {
                    return false;
                }
                stackView.pop()
            }
            else{
                var found = false;  //es wird geprüft, ob die hinzuzufügende Farbe schon vorhanden ist
                for(var i = 0; i < selectedColorLVModel.count; i++){
                   if(color==selectedColorLVModel.get(i).color){
                       found=true
                       console.log("keine doppelten Farben!")
                       break
                   }
                }
                if(found == false) {  //folgendes passiert, wenn hinzuzufügende Farbe noch nicht enthalten ist
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
                width: parent.width*0.975
                height: parent.height
                anchors.centerIn: parent
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
                       //blue
                       color: "#0000ff"

                       subcolors: '["#0f00a3","#000e9a","#000fab","#02378e",
                                    "#1500e1","#071bd9","#0000ff","#004cd7",
                                    "#301aff","#2929eb","#1e33ff","#0058f9",
                                    "#4d3de4","#4343ff","#4052ff","#2d63ff"]'
                   }

                   ListElement{
                       //turqouise
                       color: "#03D1D1"

                       subcolors: '["#00E2FF","#2BE7FF","#57ECFF","#9CF4FF",
                                    "#03D1D1","#03E6E6","#00FFFF","#25FFFF",
                                    "#00FFDE","#23FFE3","#3DFFE6","#74FFED",
                                    "#00FFC4","#20FFCC","#4EEBC6","#9FFFE9"]'
                   }

                   ListElement{
                       //lightgreen
                       color: "#16E635"

                       subcolors: '["#00FF1E","#2CFF45","#54FF68","#7FFF8E",
                                    "#16E635","#2DFD4C","#17FF3A","#00FF26",
                                    "#00FF49","#1EFF5E","#3FFF75","#69FF93",
                                    "#00FF44","#23FF5E","#37F068","#61FF8B"]'
                   }

                   ListElement{
                       //green
                       color: "green"

                       subcolors: '["#08B556","#20C56A","#49B77B","#7BCDA0",
                                                    "#06AE30","#18A63C","#40AA5A","#5F9B6E",
                                                    "#15A901","#218813","#359029","#6AA463",
                                                    "#4CAF00","#55A617","#589A26","#699A43"]'
                   }

                   ListElement{
                       //yellow
                       color: "yellow"

                       subcolors: '["#E2FF00","#E7FF2B","#EAFD59","#EFFA98",
                                    "#F7FF00","#F9FF2E","#FBFF5C","#FCFF92",
                                    "#FFF701","#FFF921","#FFFB5C","#FFFB5C",
                                    "#FFE200","#FFE835","#FFEC57","#FFF074"]'
                   }

                   ListElement{
                       //orange
                       color: "orange"

                       subcolors: '["#FF7300","#FF7300","#FFA55B","#FFA55B",
                                    "#FF4D00","#FF611D","#FF6D2E","#FF814B",
                                    "#FF8103","#FF901F","#FF9831","#FF9E3C",
                                    "#FFB300","#FFBD1F","#FFC335","#FFC949"]'
                   }

                   ListElement{
                       //red
                       color: "red"

                       subcolors: '["#FF0000","#FC2222","#FF3737","#FC5050",
                                    "#FF2B00","#EA3612","#F24D2C","#FD785D",
                                    "#FF4400","#FF5213","#FF6832","#FF7D4E",
                                    "#FF5500","#FF6314","#FE712A","#FE8143"]'
                   }

                   ListElement{
                       //purple
                       color: "purple"

                       subcolors: '["#BC00FF","#C21BFF","#CC3FFF","#D661FF",
                                    "#D500FF","#D923FD","#E14DFF","#EB83FF",
                                    "#EF00FF","#F234FF","#F666FF","#F990FF",
                                    "#FF00EF","#F522E7","#FF3FF3","#F961EF"]'
                   }

                   ListElement{
                       //grey
                       color: "grey"

                       subcolors: '["#FFFFFA","#EEEEEE","#E2E2E2","#D3D3BF",
                                    "#BABAB9","#B8B8A1","#A6A6A6","#A3A388",
                                    "#7C7C7C","#5F5F5F","#3F3F3F","#000000"]'
                   }

               }
    }
}
