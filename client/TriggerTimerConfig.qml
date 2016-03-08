import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import WebSocketConnector 1.1


Item {
    //property alias requestId: ws.requestId
    property WebSocketConnector ws;
    Component.onCompleted: {
        //console.log("rqid:" + requestId);
        ws.onMessage.connect(function(msg){
            console.log(JSON.stringify(msg));
            if(msg.hasOwnProperty("timerInterval")){
                intervalSlider.setValue = msg.timerInterval;
            }
            if(msg.hasOwnProperty("timerRand")){
                randSlider.value = msg.timerRand*100;
            }
        });
        ws.send = JSON.stringify({"getState":true});
    }

    ColumnLayout{
        //width:600;
        spacing: 20
        anchors.centerIn: parent;
        Text{
            font.pixelSize: 30
            text:qsTr("Configure Timer Trigger")
            color: "#369cb6"
        }

        ColumnLayout{
        Text{
            color: "#369cb6"
            text: qsTr("Interval:")
        }

        RowLayout{
            Layout.fillWidth: true;
            ExponentialSlider{
                id: intervalSlider
                Layout.fillWidth: true;
                maxValue: 50000
                onReadValueActiveChanged: {
                    var msg = {"timerInterval":readValue};
                    ws.send = JSON.stringify(msg);
                }
            }
            Text{
                text: Math.round(intervalSlider.readValue / 10)/100 + "s";
                color: "#369cb6";
            }
        }
        }
        ColumnLayout{
        Text{
            color: "#369cb6"
            text: qsTr("Randomness:")
        }

        RowLayout{
            Layout.fillWidth: true;
            Slider{
                id: randSlider
                Layout.fillWidth: true;
                minimumValue: 0
                maximumValue: 100
                onValueChanged: {
                    var msg = {"timerRand":value/100};
                    ws.send = JSON.stringify(msg);
                }
            }
            Text{
                text: Math.round(randSlider.value) + "%";
                color: "#369cb6";
            }
        }
        }

    }
}
