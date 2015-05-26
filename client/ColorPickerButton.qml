import QtQuick 2.0
import WebSocketConnector 1.1

Item{
    id: cpb
    property string myColor: "#FFFFFF"
    property bool onlyOne
    property alias requestId : colorPickerWSC.requestId


    property bool initFinished : false;

    onMyColorChanged: {
       if(myColor.search(",")!=-1){ //wenn nicht -1, dann mehrere Farbwerte
          buttonRect.color="#AAAAAA"
       }
       else if(myColor=="") buttonRect.color="#999999"
       else{

          buttonRect.color=myColor
          }

       if(initFinished)
            colorPickerWSC.send=JSON.stringify({"colorChanged":myColor})
       }



    WebSocketConnector{
        id: colorPickerWSC
        onMessage:{
            if(msg.colorChanged!==undefined){
                initFinished = false;
                myColor=msg.colorChanged;
                initFinished = true;
            }
        }
    }
        Rectangle{
            id:buttonRect
            x: 50
            radius: height/5
            height: 30
            width: 115
            onColorChanged: {
                var regex=/(#((([A-F]|[a-f]).....)|(..([A-F]|[a-f])...)|(....([A-F]|[a-f]).)))/i //regexp überprüft, ob Farbe hell oder dunkel ist
                if((color+"").search(regex)==-1){
                    //farbe ist dunkel
                    text.color="#FFFFFF"
                }
            }

            Text{
               id: text
               text: "Farbe wählen"
               color:"#000"
               font.pixelSize: parent.height*0.5
               verticalAlignment: Text.AlignVCenter
               horizontalAlignment: Text.AlignHCenter
               width:parent.width
               height:parent.height
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    stackView.push(Qt.resolvedUrl("ColorPicker.qml"),{"button":cpb});
                }

            }
        }
    }


