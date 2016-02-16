import QtQuick 2.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1
import QtQuick.Controls 1.2
Item{
    id:diaScene
    Component{
        id:diaListDelegate

        Rectangle {
            color:"#000000ee"
            width:parent.parent.width;
            height:text.height + 20
            MouseArea{
                anchors.fill: parent;
                property var pressedTstmp : -1;
                onPressed: {
                    pressedTstmp = new Date().getTime();
                }

                onReleased: {
                    if(pressedTstmp == -1)
                        return;
                    var dur = new Date().getTime() - pressedTstmp;
                    if(dur > 500){ //long click
                        diaList.currentIndex = index;
                        var msg = {"currentScene":id};
                        wsc.send = JSON.stringify(msg);
                        diaList.currentIndex = index;
                    }
                    else{//short click
                        diaWSC.requestId = requestId;
                        diaLayout.visible=true;
                        diaLayout.id = id;
                        diaLayout.index = index;
                    }
                }
            }

            RowLayout{
                anchors.fill: parent;

                Text{
                    anchors.centerIn: parent
                    id:text
                    text:name;
                    color:"#369cb6"
                    font.pointSize: 15;
                }
            }
        }
    }

    RowLayout{
        anchors.fill: parent;
        spacing: 30
        Item{
            Layout.fillHeight: true;
            Layout.preferredWidth: parent.width*0.3;
        ColumnLayout{
            anchors.fill: parent
        Item{
            Layout.fillHeight: true;
            Layout.fillWidth: true;
            ListView{
                id:diaList
                anchors.fill: parent;
                model: ListModel{
                    id: diaListModel
                }
                onCountChanged: setCurentIndex();


                delegate:diaListDelegate
                highlight: Rectangle { color: "#33369cb6";  }
            }
        }
        Item{
            Layout.fillWidth: true;
            Layout.preferredHeight: 50
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    addText.color= "#166c86"
                }
                onReleased: {
                    addText.color = "#369cb6"
                }
                onClicked: {
                    stackView.push(Qt.resolvedUrl("AddScene.qml"),{"requestType":"diaScene"});
                }
            }

            Text{
                id: addText
                text:"+"
                color:"#369cb6"
                font.pixelSize: 30
                anchors.centerIn: parent
            }
        }
        }
        }
        Item{
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            ColumnLayout{
                anchors.fill: parent
                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ColumnLayout{
                        id: diaLayout
                        anchors.fill: parent;
                        visible: false
                        property string requestType: ""
                        property int requestId: -1
                        property int id;
                        property int index;
                        property int monitorReqId : -1;
                        Item{
                            Layout.fillWidth: true;
                            Layout.preferredHeight: nameLabel.height + 5
                        RowLayout{
                            anchors.fill: parent
                            Text{
                                id: nameLabel
                                color:"#369cb6"
                                font.pointSize: 18;
                                text: qsTr("name:")
                            }

                        TextEdit{
                            Layout.fillWidth: true;
                            id:nameEdit
                            color:"#369cb6"
                            font.pointSize: 18;
                            wrapMode: TextEdit.WordWrap
                            onTextChanged: {
                                var msg = {"name":text};
                                diaWSC.send = JSON.stringify(msg);
                            }
                        }

                        Item {
                            id: monitorButton
                            Layout.fillHeight: true;
                            Layout.preferredWidth: height;

                            Image{
                                source: "icons/moitor.png"
                                anchors.fill: parent
                                smooth: true
                                fillMode: Image.PreserveAspectFit
                            }
                          MouseArea{
                                anchors.fill: parent
                                onClicked:{
                                    stackView.push(Qt.resolvedUrl("MonitorControl.qml"),{"requestId":diaLayout.monitorReqId} );
                                }
                            }
                        }

                        Item {
                            id: editSceneBtn
                            Layout.fillHeight: true;
                            Layout.preferredWidth: height;

                            function getFileName(){
                                if(diaLayout.requestType == "discoScene")
                                    return "DiscoScene.qml"
                                if(diaLayout.requestType == "colorScene")
                                    return "ColorScene.qml"
                                if(diaLayout.requestType == "keyFrameScene")
                                    return "KeyframeScene.qml";
                            }

                            Image{
                                source: "icons/settings.png"
                                anchors.fill: parent
                                smooth: true
                                visible: editSceneBtn.getFileName() !== ""
                                fillMode: Image.PreserveAspectFit
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked:{
                                    console.log("edit scene:" + editSceneBtn.getFileName())
                                    stackView.push(Qt.resolvedUrl(editSceneBtn.getFileName()),{"requestId":diaLayout.requestId});
                                }
                            }
                        }
                        Item{//placeholder
                            Layout.preferredWidth: 10

                        }
                        }
                        }


                    TextEdit{
                        id:descEdit
                        Layout.fillWidth: true;
                        Layout.fillHeight: true;
                        text:"asdfghjklö"
                        color:"#369cb6"
                        font.pointSize: 12;
                        wrapMode: TextEdit.WordWrap
                        onTextChanged: {
                            var msg = {"desc":text};
                            diaWSC.send = JSON.stringify(msg);
                        }
                    }
                    Item{
                        Layout.fillWidth: true;
                        Layout.preferredHeight: 50;
                        RowLayout{
                            anchors.fill: parent
                            spacing: 10
                            Text{
                                font.pointSize: 12
                                color:"#369cb6"
                                text: qsTr("Fade-in duration")
                            }

                            ExponentialSlider{
                                id:fadeDurSlider
                                Layout.fillWidth: true;
                                maxValue: 10;

                                onReadValueActiveChanged: {
                                    var msg = {"fadeInDur":readValue};
                                    diaWSC.send = JSON.stringify(msg);
                                }
                            }



                            Text{
                                id: durString
                                Layout.preferredWidth: 100
                                font.pointSize: 12
                                color:"#369cb6"
                                text: qsTr("%L1 s").arg(fadeDurSlider.readValue);
                            }


                        }
                    }
                        Item{
                            Layout.fillWidth: true;
                            Layout.preferredHeight: 25;
                            RowLayout{
                                anchors.fill: parent
                                Text{
                                    text: qsTr("Next with music:")
                                    color:"#369cb6"
                                    font.pointSize: 12
                                }
                                CheckBox{
                                    id: nextWithMusic
                                    Layout.margins: 10
                                    onCheckedChanged: {
                                        var msg = {"musicNotification":checked};
                                        wsc.send = JSON.stringify(msg);
                                    }

                                }
                                Item{
                                    Layout.fillWidth: true
                                }


                                Item{
                                    height: parent.height
                                    width: height
                                    anchors.right: parent.right
                                    anchors.rightMargin: 20
                                    Image{
                                        source: "icons/delete.png"
                                        anchors.fill: parent
                                        fillMode: Image.PreserveAspectFit
                                    }
                                    Image{
                                        id: deleteAcitve
                                        source: "icons/deleteActive.png"
                                        anchors.fill: parent
                                        fillMode: Image.PreserveAspectFit
                                        opacity: 0
                                        PropertyAnimation{
                                            id:deleteMeNowAnim
                                            target: deleteAcitve
                                            property: "opacity"
                                            running: false
                                            from: 1
                                            to:0
                                            duration: 4000
                                        }
                                    }

                                    MouseArea{
                                        anchors.fill: parent
                                        onClicked: {
                                            if(deleteMeNowAnim.running){//delete
                                                var msg = {"deleteScene":diaLayout.id};
                                                wsc.send = JSON.stringify(msg);
                                            }
                                            else
                                                deleteMeNowAnim.start();
                                        }
                                    }
                                }
                            }


                        }
                    }

                    WebSocketConnector{
                        id: diaWSC
                        onMessage:{
                            //
                            console.log(JSON.stringify(msg));
                            if(msg.desc !== undefined)
                                descEdit.text  = msg.desc;
                            if(msg.name !== undefined)
                                nameEdit.text = msg.name;
                            if(msg.fadeInDur !== undefined)
                                fadeDurSlider.setValue = msg.fadeInDur;
                            if(msg.requestId !== undefined)
                                diaLayout.requestId = msg.requestId;
                            if(msg.requestType !== undefined)
                                diaLayout.requestType = msg.requestType;
                            if(msg.hasOwnProperty("monitorReqId"))
                                diaLayout.monitorReqId = msg.monitorReqId;
                        }
                    }
                }
                Item{
                    Layout.preferredHeight: 200
                    Layout.fillWidth: true
                    RowLayout{
                        anchors.fill: parent
                    GridLayout {
                        columns: 3
                        Layout.alignment:Qt.AlignCenter
                        Layout.minimumWidth: 100
                        //Layout.preferredWidth: 600
                        Layout.maximumWidth: 600
                        Layout.preferredHeight: 200

                        DiaSceneButton{
                            imageSrc: "icons/prev.png"
                            touchedImageSrc: "icons/prevTouched.png"
                            Layout.preferredWidth: parent.width / 4;
                            onClicked: {
                                var msg = {"prevScene":true};
                                wsc.send = JSON.stringify(msg);
                            }
                        }

                        DiaSceneButton{
                            id: resetBtn
                            imageSrc: "icons/reload.png"
                            touchedImageSrc: "icons/reloadTouched.png"
                            Layout.preferredWidth: parent.width / 4;
                            rotate: true
                            onClicked: {
                                var msg = {"resetScene":true};
                                wsc.send = JSON.stringify(msg);
                            }
                        }

                        DiaSceneButton{
                            imageSrc: "icons/next.png"
                            touchedImageSrc: "icons/nextTouched.png"
                            Layout.preferredWidth: parent.width / 4;
                            onClicked: {
                                var msg = {"nextScene":true};
                                wsc.send = JSON.stringify(msg);
                            }
                        }
                    }
                   }
                }
            }
        }
    }

    WorkerScript {
            id: importer
            property var indexes;
            source: "DiaSceneImporter.js"
        }

    property int currentId : -1;
    function setCurentIndex(){
        console.log("sci called:" + currentId);
        for(var i = 0; i < diaListModel.count;i++){
            if(diaListModel.get(i).id === currentId){
                diaList.currentIndex = i;
                break;
            }
        }
    }


    WebSocketConnector{
        id: wsc
        requestType: "diaScene"
        onMessage: {
            if(msg.resetScene !== undefined)
                resetBtn.triggered();
            if(msg.currentScene !== undefined){
                currentId = msg.currentScene;
                setCurentIndex();
            }
            if(msg.scenes !== undefined){
                diaLayout.visible=false;
                importer.sendMessage({"msg":msg,"listModel":diaListModel,"diaScene":diaScene});
            }
            if(msg.hasOwnProperty("musicNotification")){
                nextWithMusic.checked = msg.musicNotification;
            }
        }
    }
}

