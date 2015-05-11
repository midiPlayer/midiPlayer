import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1


Rectangle {
    id: root
    width: parent.width
    height: parent.height

    ListView {
        id: listView
        width: parent.width
        height: parent.height

        property int dragItemIndex: -1

        model: ListModel {
            id: listModel
            /*Component.onCompleted: {
                for (var i = 0; i < 10; ++i) {
                    append({value:{
                                nr: i,
                                vElement:0
                               } });
                }
            }*/
        }

        delegate: Item {
            id: delegateItem
            width: listView.width
            height: dragRect.height

            Rectangle {
                id: dragRect
                width: listView.width
                height: column.height
                //height:60
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                Rectangle{
                    id: column
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width:parent.width
                    height:row.height + optionBox.height
                    color: "#333"
                    border.color: "#000"
                    border.width: 1

                    MouseArea{
                        anchors.fill: row
                        onPressed: {
                            if(optionBox.height != 0)
                                optionBox.height = 0;
                            else
                                optionBox.height = optionBox.implicitHeight;
                        }
                    }



                RowLayout{
                    id: row
                    z: 2
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width:parent.width
                    height:100


                    Rectangle{
                        height: parent.height
                        width:100
                        anchors.left: parent.left
                        color: "green"

                        MouseArea {
                            id: mouseArea
                            anchors.fill:parent
                            drag.target: dragRect
                            drag.axis: Drag.YAxis
                            onPressed: {
                                dragRect.dragStart = delegateItem.y;
                                console.log(dragRect.dragStart);
                            }
                        }
                    }

                    Text {
                        text: modelData.name
                        color:"#fff"
                    }


                    PushLockBtn{
                        id: soloBtn
                        onStateOn: {
                            mtBtn.setOff(true);
                        }
                        anchors.right: mtBtn.left;
                        anchors.margins: 10;
                    }

                    PushLockBtn{
                        id:mtBtn
                        mute: "true"
                        onStateOn: {
                            soloBtn.setOff(true);
                            muteStateChanged(modelData.sceneId,true);
                        }
                        onStateOff: {
                            modelData.mute = false;
                            muteStateChanged(modelData.sceneId,false);
                        }

                        isOn: modelData.mute;

                        anchors.right: parent.right;
                        anchors.margins: 10;
                    }
                }



                    Item{
                        id:optionBox
                        anchors.top: row.bottom
                        anchors.left: parent.left
                        height:0
                        //implicitHeight: beatScene.height
                        property Item beatScene;
                        width:parent.width
                        Behavior on height{
                           NumberAnimation{
                               duration: 200
                               onRunningChanged: {
                                    if (running) {//start
                                        if(height != 0)
                                            optionBox.beatScene.visible = false;
                                    }
                                    else{//stoped
                                        if(optionBox.height != 0)
                                            optionBox.beatScene.visible = true;
                                    }
                               }
                           }
                        }
                        onHeightChanged: {

                        }


                        Item{
                            id:placeholder
                        }
                        Component.onCompleted: {
                            var filename = "";
                            if(modelData.requestType === "beatScene1")
                                filename = "BeatScene.qml";
                            if(modelData.requestType === "falshScene")
                                filename = "FlashScene.qml";
                            if(filename != ""){
                            var obj = Qt.createComponent(filename,0,this);
                                if (obj.status === Component.Ready)
                                        finishCreation(obj);
                                    else
                                        obj.statusChanged.connect(finishCreation(obj));
                            }
                        }

                        function finishCreation(obj) {
                            var incubator = obj.incubateObject(placeholder,{"visible":false,"requestId":modelData.providerId});
                           if (incubator.status !== Component.Ready) {
                               incubator.onStatusChanged = function(status) {
                                   if (status === Component.Ready) {
                                       optionBox.implicitHeight = incubator.object.height;
                                       beatScene = incubator.object;
                                   }
                               }
                           }
                        }

                    }

            }

                property int dragStart: 0
                property int jumpedItems: 0
                onYChanged: {
                    if(!dragRect.Drag.active)
                        return;
                    var nextHeight = 100000;
                    if(listView.itemAt(dragRect.width/2,y+height+listView.contentY) !== null)
                        nextHeight = listView.itemAt(dragRect.width/2,y+height+listView.contentY).height;

                    var prevHeight = 100000;
                    if(listView.itemAt(dragRect.width/2,y+listView.contentY) !== null)
                        prevHeight = listView.itemAt(dragRect.width/2,y+listView.contentY).height;

                   if(dragRect.y + listView.contentY - dragStart > nextHeight * 0.75
                           && index < listView.count-1
                           && (listView.itemAt(dragRect.width/2,y+height+listView.contentY) !== delegateItem)){

                        dragStart = dragStart + nextHeight;
                        listView.model.move(index,index+1,1);
                       jumpedItems++;
                   }
                   if(dragRect.y + listView.contentY - dragStart < -prevHeight * 0.75
                           && index != 0
                           && (listView.itemAt(dragRect.width/2,y+height+listView.contentY) !== delegateItem)){
                        listView.model.move(index,index-1,1);
                       dragStart = dragStart - prevHeight;
                       if(index == 0){
                           mouseArea.drag.minimumY = 0;
                       }
                   }
                 }


                states: [
                    State {
                        when: dragRect.Drag.active
                        ParentChange {
                            target: dragRect
                            parent: root
                        }

                        AnchorChanges {
                            target: dragRect
                            anchors.horizontalCenter: undefined
                            anchors.verticalCenter: undefined
                        }
                    }
                ]

                Drag.active: mouseArea.drag.active
                Drag.hotSpot.x: dragRect.width / 2
                Drag.hotSpot.y: dragRect.height / 2
            }
        }
    }


    WorkerScript {
            id: importer
            source: "DiscoSceneImport.js"
        }


    WebSocketConnector{
        id: ws
        requestType: "discoScene"
        onMessage: {
            importer.sendMessage({"msg":msg,"listModel":listModel});
        }
    }

    function muteStateChanged(id,stateP){
        var msg = new Object();
        msg.muteChanged = new Object();
        msg.muteChanged.sceneId = id;
        msg.muteChanged.state = stateP;
        ws.send = JSON.stringify(msg);
    }

}
