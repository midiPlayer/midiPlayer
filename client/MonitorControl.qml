import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import WebSocketConnector 1.1


Item {
    property alias requestId: ws.requestId

    ColumnLayout{
        //width:600;
        spacing: 20
        anchors.centerIn: parent;
        Text{
            font.pixelSize: 30
            text:qsTr("Configure Monitor behaviour")
            color: "#369cb6"
        }

        ColumnLayout{
        Text{
            color: "#369cb6"
            text: qsTr("List of Ressources:")
        }

        RowLayout{
            Layout.fillWidth: true;
            TextArea{


                id: ressources
                Layout.fillWidth: true;
                focus:true;
                Keys.onReleased: {
                    var msg = new Object();
                    msg.key_ressources = text.split("\n");
                    ws.send = JSON.stringify(msg);
                }

                function setPassiv(newText){
                    text = newText;
                }
            }
        }
        }
        ColumnLayout{
            Text{
                color: "#369cb6"
                text: qsTr("Trigger sources:")
            }
            TriggerSourceBtn{
                id:triggerBtn
            }
        }
        RowLayout{
            Layout.fillWidth: true;
            CheckBox{
                id:randomCheckb
                onCheckedChanged: {
                    activeChange();
                }

                signal activeChange();
                onActiveChange: {
                    var msg = new Object();
                    msg.key_random = checked;
                    ws.send = JSON.stringify(msg);
                }

                function setPassiv(isChecked){
                    //supressNotif = true;
                    checked = isChecked;
                }

            }
            Text {
                 text: qsTr("select next image randomly")
                 color: "#369cb6"
           }
        }
        ColumnLayout{
            Text {
                text: qsTr("Fade Over Time for triggerd changes")
                color: "#369cb6"
            }
            ExponentialSlider{
                id:beatDurSlider
                Layout.fillWidth: true;
                maxValue: 10;

                onReadValueActiveChanged: {
                    var msg = {"key_beatFadeTime":readValue};
                    ws.send = JSON.stringify(msg);
                }
            }
        }
    }

    WebSocketConnector{
        id: ws
        onMessage: {
            console.log(JSON.stringify(msg));
            if(msg.hasOwnProperty("key_random")){
                randomCheckb.setPassiv(msg.key_random);
            }
            if(msg.hasOwnProperty("key_ressources")){
                ressources.text = "";
                for(var i = 0; i < msg.key_ressources.length;i++){
                    ressources.setPassiv(ressources.text + (i == 0 ? "" : "\n") + msg.key_ressources[i]);
                }
            }
            if(msg.hasOwnProperty("key_trigger")){
                triggerBtn.requestId = msg.key_trigger;
            }

            if(msg.hasOwnProperty("key_beatFadeTime"))
                beatDurSlider.setValue = msg.key_beatFadeTime;
        }
    }
}
