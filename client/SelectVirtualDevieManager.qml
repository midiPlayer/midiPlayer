import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import WebSocketConnector 1.1

Item{
    property alias requestId : ws.requestId

    width:parent.width
    height: parent.height

    ScrollView{
        anchors.fill: parent;
    ColumnLayout{
            width: parent.width;
            SelectVirtualDeviceManagerGroup{
                id:rgbGroup
                typeName: qsTr("RGB-Devices")
                typeId: 1
                onDataChanged: {
                    sendData();
                }
                wsc:ws
            }
            SelectVirtualDeviceManagerGroup{
                id:rgbwGroup
                typeName: qsTr("RGBW-Devices")
                typeId: 2
                onDataChanged: {
                    sendData();
                }
                wsc:ws
            }
            SelectVirtualDeviceManagerGroup{
                id:singleGroup
                typeName: qsTr("Single-Devices")
                typeId: 0
                onDataChanged: {
                    sendData();
                }
                wsc:ws
            }
            SelectVirtualDeviceManagerGroup{
                id:beamerGroup
                typeName: qsTr("Beamer")
                typeId: 3
                onDataChanged: {
                    sendData();
                }
                wsc:ws
            }
        }
    }

    function sendData(){
        var model = beamerGroup.model;
        for(var i = 0;i< model.count;i++){
            if(model.get(i).accepted)
                console.log(model.get(i).devId);
        }
    }

    WorkerScript {
            id: worker
            source: "SelectVirtualDevieManagerImporter.js"
   }

    WebSocketConnector{
        id: ws
        onMessage: {
            console.log(JSON.stringify(msg));
            if(msg.hasOwnProperty("devices")){
                for(var i = 0; i < msg.devices.length; i++){
                    var type = msg.devices[i].type;
                    if(type === rgbGroup.typeId){
                        worker.sendMessage({"model":rgbGroup.model,"device": msg.devices[i],"acc_dev":msg.acc_dev});
                    }
                    else if(type === rgbwGroup.typeId){
                        worker.sendMessage({"model":rgbwGroup.model,"device": msg.devices[i],"acc_dev":msg.acc_dev});
                    }
                    else if(type === beamerGroup.typeId){
                        worker.sendMessage({"model":beamerGroup.model,"device": msg.devices[i], "acc_dev":msg.acc_dev});
                    }
                    else if(type === singleGroup.typeId){
                        worker.sendMessage({"model":singleGroup.model,"device": msg.devices[i],"acc_dev":msg.acc_dev});
                    }
                    else{
                        console.log("ERROR! unknown device type! - Not implemented in gui.");
                        continue;
                    }
                }
            }
        }
    }

}
