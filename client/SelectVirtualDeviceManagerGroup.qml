import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import WebSocketConnector 1.1

ColumnLayout{
    property alias typeName : devIdText.text;
    property int typeId;
    property alias model : grid.model
    property alias typeAccepted : typeCb.checked
    signal dataChanged();

    property WebSocketConnector wsc;

    Layout.leftMargin: 20

RowLayout{
    spacing: 20
    Text{
        id:devIdText
        color:"#369cb6"
        font.pointSize: 16
        text:typeName
    }

    CheckBox{
        id:typeCb
        text:qsTr("select all")
        onCheckedChanged: {
            checkChanged();
            if(checked)
                var msg = {"addAccType" : typeId}
            else
                var msg = {"rmAccType" : typeId}
            wsc.send = JSON.stringify(msg);
        }
        signal checkChanged();

        Component.onCompleted: {
            wsc.onMessage.connect(function(msg){
             if(msg.hasOwnProperty("addAccType") && msg.addAccType === typeId){
                 checked = true;
                 checkChanged();
             }
             if(msg.hasOwnProperty("rmAccType") && msg.rmAccType === typeId){
                 checked = false;
                 checkChanged()
             }
             if(msg.hasOwnProperty("acc_types")){
                 checked = msg.acc_types.indexOf(typeId) !== -1;
                 checkChanged();
             }
            });
        }
    }
}


GridView {
        id: grid
        cellWidth: 80; cellHeight: 80
        Layout.fillWidth: true;

        height:200

        model: ListModel{
        }


        delegate: Component {
            id:delegate
            Item {
                width: grid.cellWidth; height: grid.cellHeight
                states: [
                    State {
                        name: "checked"
                        PropertyChanges {
                            target: coloredRect
                            color:"#ffffff"

                        }
                    },
                    State {
                        name: "dissabled"
                        PropertyChanges {
                            target: coloredRect
                            color:"#666666"

                        }
                    }
                ]

                Component.onCompleted: {
                    if(typeCb.checked)
                        state = "dissabled";
                    else if(accepted)
                        state = "checked";
                    else{
                        state = "";
                    }


                   typeCb.checkChanged.connect(function(){
                       if(typeCb.checked){
                            state = "dissabled"
                       }
                       else{
                           state = accepted ? "checked" : "";
                       }
                   });
                }


                transitions: Transition {
                        ColorAnimation {  duration: 100; }
                    }
                Rectangle{
                    id:coloredRect
                    color:"#22000000"
                    radius: 2
                    width: parent.width - 4;
                    height: parent.height - 4;
                    anchors.centerIn: parent;
                    Text {
                        text: devId;
                        anchors.centerIn: parent
                        color: "#369cb6";
                    }
                }
                MouseArea{
                    anchors.fill: parent;
                    onClicked: {
                        if(typeCb.checked)
                            return;
                        if(parent.state == ""){
                            parent.state = "checked";
                            model.accepted = true;
                            var msg = {"addAccepted":devId};
                            wsc.send = JSON.stringify(msg);
                        }
                        else{
                            parent.state = "";
                            model.accepted = false;
                            var msg = {"rmAccepted":devId};
                            wsc.send = JSON.stringify(msg);
                        }
                    }

                    //state: ""
                    Component.onCompleted: {

                        wsc.onMessage.connect(function(msg){
                            if(msg.hasOwnProperty("addAccepted") && msg.addAccepted === devId){
                                accepted = true;
                                if(!typeCb.checked)
                                    state = "checked";
                            }
                            if(msg.hasOwnProperty("rmAccepted") && msg.rmAccepted === devId){
                                accepted = false;
                                if(!typeCb.checked)
                                    state = "";
                            }
                        });
                    }
                }
            }
        }
    }
    }

