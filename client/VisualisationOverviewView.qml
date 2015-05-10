import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.WebSockets 1.0
import QtQuick.Layouts 1.1


Rectangle {
    id: root
    width: parent.width
    height: parent.height
    property string address:""
    MessageDialog{
        text: address;
        Component.onCompleted: visible = true
    }
    ListView {
        id: listView
        width: parent.width
        height: parent.height

        property int dragItemIndex: -1

        model: ListModel {
            Component.onCompleted: {
                for (var i = 0; i < 10; ++i) {
                    append({value:{
                                nr: i,
                                vElement:0
                               } });
                }
            }
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
                color: "#eee"
                border.color: "#999"
                border.width: 1
                Component.onCompleted: {
                    modelData.vElement=dragRect.height;
                    console.log("set");
                }

                Rectangle{
                    id: column
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width:parent.width
                    height:row.height + optionBox.height
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
                        text: modelData.nr
                    }


                    PushLockBtn{
                        id: soloBtn
                        onStateOn: {
                          mtBtn.setOff(true);
                        }
                        mute: "true"
                        anchors.right: mtBtn.left;
                        anchors.margins: 10;
                    }

                    PushLockBtn{
                        id:mtBtn
                        onStateOn: {
                            soloBtn.setOff(true);
                        }
                        anchors.right: parent.right;
                        anchors.margins: 10;
                    }
                }



                    Rectangle{
                        id:optionBox
                        anchors.top: row.bottom
                        anchors.left: parent.left
                        height:0
                        implicitHeight: 100
                        color:"red"
                        width:parent.width
                        Behavior on height{
                           NumberAnimation{
                               duration: 300
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

                        console.log("jetzt umspringen");
                        dragStart = dragStart + nextHeight;
                        listView.model.move(index,index+1,1);
                       jumpedItems++;
                   }
                   if(dragRect.y + listView.contentY - dragStart < -prevHeight * 0.75
                           && index != 0
                           && (listView.itemAt(dragRect.width/2,y+height+listView.contentY) !== delegateItem)){
                        console.log("jetzt nach unten umspringen");
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


}
