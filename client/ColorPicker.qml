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

                subcolors: '["#c8b9ff","#2d00d0","#c7e3ff","#02378e",
                             "#9273ff","#071bd9","#91c8ff","#0062c5",
                             "#6e46ff","#210097","#5aacff","#003366",
                             "#3700ff","#0d003b","#007fff","#001831"]'
            }

            ListElement{
                //turqouise
                color: "#0fdad6"
                subcolors: '["#00dcff","#00d7ff","#00ffbd","#00ffeb",
                             "#00a1bb","#29bcd7","#00b283","#30d4c7",
                             "#007183","#4ba1b1","#008a66","#4aafa7",
                             "#003d46","#5d797f","#004936","#5b7876"]'
            }

            ListElement{
                //green
                color: "#0cff1c"
                subcolors: '["#00ff5f","#12ff00","#8bff00",
                             "#009638","#3ecb33","#8cc34b",
                             "#00511e","#2c6628","#4f6a30"]'
            }

            ListElement{
                //yellow
                color: "#f8ff0c"
                subcolors: '["#d2ff00","#f5ff00","#fff57f",
                             "#9cb238","#c2c658","#ffeb00",
                             "#687432","#5e601d","#7b7106"]'
            }

            ListElement{
                //orange
                color: "#ffae00"
                subcolors: '["#ffd700","#ffb300",
                             "#fff09f","#ac6400"]'
            }

            ListElement{
                //red
                color: "#ff0014"
                subcolors: '["#ffa06c","#ff8580","#ff5678",
                             "#ff5b00","#ff0b00","#ff0032",
                             "#973600","#a20700","#9b001f"]'
            }

            ListElement{
                //purple
                color: "#ff00f6"
                subcolors: '["#ffdaf4","#b0007b","#fdcfff","#f200ff",
                             "#ffa9e5","#770054","#fa9dff","#be00c7",
                             "#ff66d2","#520039","#f87aff","#84008b",
                             "#ff00b3","#310022","#f538ff","#520057"]'
            }

            ListElement{
                //grey
                color: "#9f9f9f"
                subcolors: '["#ffffff","#dddddd","#bbbbbb",
                             "#999999","#777777","#555555",
                             "#333333","#111111","#000000"]'
            }

        }
    }
}
