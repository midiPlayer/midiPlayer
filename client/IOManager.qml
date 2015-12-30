import QtQuick 2.0
import WebSocketConnector 1.1
import QtQuick.Dialogs 1.2
import FileOutput 1.0
import FileInput 1.0

Item {
    WebSocketConnector {
        id: ws
        onConnectionSucceded: {
            requestType = "FileIO"
            console.log("hallo123")
        }
        onMessage: {
            fileOut.content = JSON.stringify(msg.export);
        }

    }

    FileOutput{
        id: fileOut
    }

    FileInput{
        id: fileIn
        onContentRead: {
            console.log(JSON.stringify(JSON.parse(content)));
            var msg = Object();
            msg.open = JSON.parse(content);
            ws.send = JSON.stringify(msg);

        }
    }

    FileDialog{
        id: fileDialogOutput
        title: qsTr("Please select a filename")
        folder: shortcuts.documents
        onAccepted:{
            String.prototype.endsWith = function(suffix) {
                return this.indexOf(suffix, this.length - suffix.length) !== -1;
            };
            var path = fileDialogOutput.fileUrl.toString().replace("file://","");
            if (!path.endsWith(".lcf"))
                path += ".lcf";
            fileOut.path = path;
            }
        selectMultiple: false
        selectExisting: false
        selectFolder: false
        nameFilters: ["LightConfig files (*.lcf)", "All files (*)"]
    }

    FileDialog{
        id: fileDialogInput
        title: qsTr("Please choose a file")
        folder: shortcuts.documents
        onAccepted:{
            String.prototype.endsWith = function(suffix) {
                return this.indexOf(suffix, this.length - suffix.length) !== -1;
            };
            var path = fileDialogInput.fileUrl.toString().replace("file://","");
            if (!path.endsWith(".lcf"))
                path += ".lcf";
            fileIn.path = path;
            }
        selectMultiple: false
        selectExisting: true
        selectFolder: false
        nameFilters: ["LightConfig files (*.lcf)", "All files (*)"]
    }

    function save(){
        if (ws.connected){

            var msg = Object();
            msg.save = true;
            ws.send = JSON.stringify(msg);

            fileOut.path = "";
            fileOut.content = "";
            fileDialogOutput.open();
        }
    }

    function open(){
        if (ws.connected){

            fileIn.path = "";
            fileDialogInput.open();
        }
    }
}
