import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1


Item {
    property alias requestId : ws.requestId
    width: parent.width
    height: parent.height
    id: discoScene
    property PushLockBtn currentSolo: null
    ColumnLayout{
        anchors.fill: parent;
    Item{
        Layout.fillWidth: true;
        Layout.fillHeight: true;

        ListView {
            anchors.fill: parent;
            id: discolistView

        property int dragItemIndex: -1

        model: ListModel {
            id: listModel

            function rowMovedManualy(){
                var msg = new Object();
                msg.orderChanged = [];
                for(var i = 0;i < count; i++){
                    msg.orderChanged.push(listModel.get(i).modelData.sceneId);
                }
                ws.send = JSON.stringify(msg);
            }

        }

        delegate: Item {
            id: delegateItem
            width: discolistView.width
            height: dragRect.height

            WebSocketConnector{
                id: dlegateWs
                requestType: "discoScene"
                onMessage: {
                    if(msg.fusionTypeChanged !== undefined){
                         if(modelData.sceneId === msg.fusionTypeChanged.sceneId){
                             console.log("new combo box state:");
                             for(var i  =0; i < fusionTypeModel.count;i++){
                                 if(fusionTypeModel.get(i).name === msg.fusionTypeChanged.fusionType){
                                     fusionTypeCombo.currentIndex = i;
                                     break;
                                 }
                             }
                         }
                    }
                    if(msg.muteChanged !== undefined){
                        if(modelData.sceneId === msg.muteChanged.sceneId){
                             console.log("new mute state:");
                            if(msg.muteChanged.state)
                                activeBtn.setOff(false)
                            else
                                activeBtn.setOn(false)
                        }
                    }
                    if(msg.soloChanged !== undefined){
                        if(modelData.sceneId === msg.soloChanged.sceneId){
                             console.log("new solo state:");
                            if(msg.soloChanged.state){
                                soloBtn.setOn(false);
                                soloBtn.switchOffOthers();
                            }
                            else
                                soloBtn.setOff(false)
                        }
                    }
                }
            }

            Item {
                id: dragRect
                width: discolistView.width
                height: column.height
                //height:60
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                Rectangle{
                    id: column
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.right: parent.right
                    width:parent.width
                    height:discoRow.height + optionBox.height
                    color: "#33000000"
                    border.color: "#55000000"
                    border.width: 1
                    anchors.margins: 0

                Item{
                    id:discoRow
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height:100


                RowLayout{
                    z: 2

                    anchors.fill: parent
                    spacing: 1


                    Item{
                        id: dragger
                        height: parent.height
                        Layout.preferredWidth: 100

                        Image {
                            anchors.centerIn: parent
                            source: "icons/move.png"
                            fillMode: scale
                            width: parent.width*0.5
                            height: parent.height * 0.5

                        }

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


                    Item{
                        Layout.fillWidth: true;
                        Layout.fillHeight: true
                    Text {
                        text: modelData.name
                        color:"#fff"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressed: {
                            if(optionBox.height != 0)
                                optionBox.height = 0;
                            else
                                optionBox.height = optionBox.implicitHeight;
                        }
                    }
                    }

                    ComboBox {
                        id:fusionTypeCombo
                        Layout.preferredWidth: 200
                        model: fusionTypeModel
                        currentIndex: modelData.fusionTypeId
                        onActivated: {
                            var msg = new Object();
                            msg.fusionTypeChanged = new Object();
                            msg.fusionTypeChanged.sceneId = modelData.sceneId;
                            msg.fusionTypeChanged.fusionType = model.get(index).name;
                            ws.send = JSON.stringify(msg);
                        }
                    }

                    Button{
                        id: delBtn
                        text:qsTr("Delete")
                        onClicked: {
                            var msg = new Object();
                            msg.deleteScene = modelData.sceneId;
                            ws.send = JSON.stringify(msg);
                            importer.sendMessage({"msg":msg,"listModel":listModel});
                        }
                    }

                    Item{
                        Layout.preferredWidth: soloBtn.width+5;
                        height: parent.height;
                        PushLockBtn{
                            id: soloBtn
                            onStateOn: {
                                soloStateChanged(modelData.sceneId,true);
                                switchOffOthers();
                            }
                            onStateOff: {
                                soloStateChanged(modelData.sceneId,false);
                            }

                            function soloStateChanged(id,stateP){
                                var msg = new Object();
                                msg.soloChanged = new Object();
                                msg.soloChanged.sceneId = id;
                                msg.soloChanged.state = stateP;
                                ws.send = JSON.stringify(msg);
                            }
                            function switchOffOthers(){
                                if(discoScene.currentSolo !== null && discoScene.currentSolo != soloBtn){
                                    discoScene.currentSolo.setOff(false);
                                }
                                discoScene.currentSolo = soloBtn;
                            }

                            anchors.centerIn: parent;
                        }
                    }

                    Item{
                    Layout.preferredWidth: activeBtn.width+5;
                    height: parent.height;
                    PushLockBtn{
                        anchors.centerIn: parent;
                        id:activeBtn
                        mute: true
                        onStateOn: {
                            muteStateChanged(modelData.sceneId,false);
                        }
                        onStateOff: {
                            muteStateChanged(modelData.sceneId,true);
                        }

                        isOn: !modelData.mute;

                        anchors.right: parent.right;
                        anchors.margins: 10;

                        function muteStateChanged(id,stateP){
                            var msg = new Object();
                            msg.muteChanged = new Object();
                            msg.muteChanged.sceneId = id;
                            msg.muteChanged.state = stateP;
                            ws.send = JSON.stringify(msg);
                        }
                    }
                    }
                }

                }


                    Item{
                        id:optionBox
                        anchors.top: discoRow.bottom
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
                            if(modelData.requestType === "colorScene")
                                filename = "ColorScene.qml";
                            if(modelData.requestType === "musicScene")
                                filename = "MusicScene.qml";

                            if(modelData.requestType === "colorWaveScene")
                                filename = "ColorWaveScene.qml";
                            if(modelData.requestType === "colorWheel")
                                filename = "ColorWheelScene.qml";
                            if(modelData.requestType === "keyFrameScene")
                                filename = "KeyframeSceneDiscoWrapper.qml";
                            if(filename != ""){
                            var obj = Qt.createComponent(filename,0,this);
                                if (obj.status === Component.Ready){
                                        finishCreation(obj);
                                }
                                else{
                                    obj.statusChanged.connect(finishCreation(obj));
                                }
                            }
                        }

                        property var incubator;
                        function finishCreation(obj) {
                           incubator = obj.incubateObject(placeholder,{"visible":false,"requestId":modelData.providerId});
                           if (incubator.status !== Component.Ready) {
                               incubator.onStatusChanged = function(status) {
                                   if (status === Component.Ready) {
                                       optionBox.implicitHeight = incubator.object.height;
                                       beatScene = incubator.object;
                                   }
                               }
                           }
                           else{
                               optionBox.implicitHeight = incubator.object.height;
                               beatScene = incubator.object;
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
                    if(discolistView.itemAt(dragRect.width/2,y+height+discolistView.contentY) !== null)
                        nextHeight = discolistView.itemAt(dragRect.width/2,y+height+discolistView.contentY).height;

                    var prevHeight = 100000;
                    if(discolistView.itemAt(dragRect.width/2,y+discolistView.contentY) !== null)
                        prevHeight = discolistView.itemAt(dragRect.width/2,y+discolistView.contentY).height;

                   if(dragRect.y + discolistView.contentY - dragStart > nextHeight * 0.75
                           && index < discolistView.count-1
                           && (discolistView.itemAt(dragRect.width/2,y+height+discolistView.contentY) !== delegateItem)){

                        dragStart = dragStart + nextHeight;
                        discolistView.model.move(index,index+1,1);
                        discolistView.model.rowMovedManualy();
                       jumpedItems++;
                   }
                   if(dragRect.y + discolistView.contentY - dragStart < -prevHeight * 0.75
                           && index != 0
                           && (discolistView.itemAt(dragRect.width/2,y+height+discolistView.contentY) !== delegateItem)){
                        discolistView.model.move(index,index-1,1);
                       discolistView.model.rowMovedManualy();
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
                            parent: discoScene
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
    RowLayout{
        Layout.preferredHeight: 50;
        Layout.fillWidth: true;
        Button{
            text: "+"
            onClicked: {
                stackView.push(Qt.resolvedUrl("AddScene.qml"),{"requestId":requestId});
            }
        }
    }

    }
    ListModel{
        id: fusionTypeModel
        ListElement{
            name:"av"
            text: qsTr("average")
        }
        ListElement{
            name:"max"
            text: qsTr("maximum")
        }
        ListElement{
            name:"min"
            text: qsTr("minimum")
        }
        ListElement{
            name:"maxg"
            text: qsTr("overall maximum")
        }
        ListElement{
            name:"ming"
            text: qsTr("overall minimum")
        }
        ListElement{
            name:"override"
            text: qsTr("override")
        }
    }


    WorkerScript {
            id: importer
            source: "DiscoSceneImport.js"
        }


    WebSocketConnector{
        id: ws
        onMessage: {

            importer.sendMessage({"msg":msg,"listModel":listModel,"fusionTypeModel":fusionTypeModel});
        }
    }



}
