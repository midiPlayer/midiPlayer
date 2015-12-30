import QtQuick 2.0
import WebSocketConnector 1.1
import QtQuick.Dialogs 1.2
import FileIO 1.0

Item {
    WebSocketConnector {
        id: ws
        onConnectionSucceded: {
            requestType = "FileIO"
            console.log("hallo123")
        }
        onMessage: {
            fileio.content = JSON.stringify(msg.export);
            //console.log(JSON.stringify(msg))
        }

    }

    FileIO{
        id: fileio
    }

    FileDialog{
        id: fileDialog
        title: qsTr("Please choose a file")
        folder: shortcuts.documents
        onAccepted:{
            String.prototype.endsWith = function(suffix) {
                return this.indexOf(suffix, this.length - suffix.length) !== -1;
            };
            var path = fileDialog.fileUrl.toString().replace("file://","");
            if (!path.endsWith(".lcf"))
                path += ".lcf";
            fileio.path = path;
            }
        selectMultiple: false
        selectExisting: false
        selectFolder: false
        nameFilters: ["LightConfig files (*.lcf)", "All files (*)"]

    }

    function save(){
        if (ws.connected){

            var msg = Object();
            msg.save = true;
            ws.send = JSON.stringify(msg);

            fileio.path = "";
            fileio.content = "";
            fileDialog.open();
        }
    }
}
