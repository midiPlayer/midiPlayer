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
                            id: editSceneBtn
                            Layout.fillHeight: true;
                            Layout.preferredWidth: height;

                            function getFileName(){
                                if(diaLayout.requestType == "discoScene")
                                    return "DiscoScene.qml"
                                if(diaLayout.requestType == "colorScene")
                                    return "ColorScene.qml"
                                return "";
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
                                text: qsTr("fade in duration")
                            }

                            Slider{
                                id:fadeDurSlider
                                minimumValue: 0
                                maximumValue: Math.log(10 + 1)
                                Layout.fillWidth: true;
                                function setDur(dur){
                                    value = Math.log(dur + 1);
                                }
                                property double duration;

                                onValueChanged: {
                                    duration =(Math.exp(value)-1);
                                }

                                onDurationChanged: {
                                    if(pressed){
                                        var msg = {"fadeInDur":duration};
                                        diaWSC.send = JSON.stringify(msg);
                                    }
                                }

                            }

                            Text{
                                id: durString
                                Layout.preferredWidth: 100
                                font.pointSize: 12
                                color:"#369cb6"
                                text: qsTr("%L1 s").arg(fadeDurSlider.duration);
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
                                fadeDurSlider.setDur(msg.fadeInDur)
                            if(msg.requestId !== undefined)
                                diaLayout.requestId = msg.requestId;
                            if(msg.requestType !== undefined)
                                diaLayout.requestType = msg.requestType;
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
                importer.sendMessage({"msg":msg,"listModel":diaListModel,"diaScene":diaScene});
            }
        }
    }
}
