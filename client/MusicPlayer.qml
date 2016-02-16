import QtQuick 2.0
import WebSocketConnector 1.1
import QtQuick.Controls 1.2

Item {
    height:combo.height
    width: combo.width
    ListModel{
        id: musicModel
        ListElement {
            text: "-- Lieder werden geladen ... --"
            path: ""
        }
        onCountChanged: {
            updatePath();
        }
    }

    property alias requestId : ws.requestId
    ComboBox{
        id: combo
        width: 200
        model: musicModel
        property bool surpressNotify : false;
        onCurrentIndexChanged: {
            if(surpressNotify){
                surpressNotify = false;
                return
            }
            var msg = new Object();
            msg.setPath = musicModel.get(currentIndex).path;
            ws.send = JSON.stringify(msg);
        }
        onModelChanged: {
            console.log("model changed")
        }
    }

    WorkerScript {
            id: songWorker
            source: "MusicPlayerWorker.js"
   }

    property string path : "";

    function updatePath() {
        for(var i = 0; i < musicModel.count;i++){
            if(musicModel.get(i).path === path){
                if(combo.currentIndex != i){
                    combo.surpressNotify = true;
                    combo.currentIndex = i;
                }
                break;
            }
        }
    }




    WebSocketConnector{
        id: ws

        onMessage: {

          songWorker.sendMessage({"model":musicModel,"msg":msg});
          if(msg.hasOwnProperty("setPath")){
            path =  msg.setPath;
              updatePath(path);
            console.log(JSON.stringify(msg))
           }
        }
    }
}
